# Complete Roadmap: Retained-Mode UI System on Vulkan

## Overview Timeline

**Phase 1:** Foundation (Weeks 1-2) - Vulkan + Basic Architecture  
**Phase 2:** Core UI System (Weeks 3-6) - Widget tree, events, rendering  
**Phase 3:** Essential Widgets (Weeks 7-9) - Buttons, text, input  
**Phase 4:** Layout & Windows (Weeks 10-12) - Layout engine, window management  
**Phase 5:** Advanced Features (Weeks 13-16) - Themes, animations, polish  
**Phase 6:** Integration (Weeks 17-18) - Real application features  

---

# PHASE 1: FOUNDATION (Weeks 1-2)

## Week 1: Vulkan 3D Rendering Pipeline

### Day 1-2: Vulkan Instance & Device Setup
**Goal:** Initialize Vulkan properly

**Files to create:**
```
src/render/VulkanContext.h
src/render/VulkanContext.cpp
```

**Tasks:**
1. Create Vulkan instance
2. Enable validation layers (debug mode)
3. Select physical device (GPU)
4. Create logical device
5. Set up queue families (graphics queue)
6. Create command pool
7. Error handling and logging

**Deliverable:** Vulkan initialized, no errors in validation layers

---

### Day 3-4: Swap Chain & Render Pass
**Goal:** Set up the presentation pipeline

**Files to create:**
```
src/render/SwapChain.h
src/render/SwapChain.cpp
```

**Tasks:**
1. Query swap chain support
2. Choose surface format, present mode, extent
3. Create swap chain
4. Get swap chain images
5. Create image views
6. Create render pass (color attachment)
7. Create framebuffers (one per swap chain image)
8. Handle window resize (basic)

**Deliverable:** Can clear screen to a color

---

### Day 5-7: First Triangle (3D Pipeline)
**Goal:** Render geometry

**Files to create:**
```
src/render/GraphicsPipeline.h
src/render/GraphicsPipeline.cpp
src/render/Shader.h
src/render/Shader.cpp
shaders/basic.vert
shaders/basic.frag
```

**Tasks:**
1. Load SPIR-V shaders (compile GLSL to SPIR-V)
2. Create graphics pipeline:
   - Vertex input state
   - Input assembly (triangles)
   - Viewport and scissor
   - Rasterizer
   - Multisampling
   - Color blending
3. Create vertex buffer
4. Create index buffer (optional)
5. Record command buffers
6. Render loop with semaphores
7. Camera system (basic orbit camera)

**Shaders:**
```glsl
// basic.vert
#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

void main() {
    gl_Position = ubo.proj * ubo.view * vec4(inPosition, 1.0);
    fragColor = inColor;
}

// basic.frag
#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}
```

**Deliverable:** Rotating colored triangle/cube on screen

---

## Week 2: Application Architecture & Input

### Day 8-9: Application Framework
**Goal:** Proper application structure

**Files to create:**
```
src/core/Application.h
src/core/Application.cpp
src/core/Window.h
src/core/Window.cpp
src/core/Input.h
src/core/Input.cpp
```

**Application class:**
```cpp
class Application {
public:
    void run();
    void shutdown();
    
private:
    void init();
    void mainLoop();
    void cleanup();
    
    Window* window;
    VulkanContext* vulkanContext;
    Renderer* renderer;
    
    bool running = true;
};
```

**Window class:**
```cpp
class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();
    
    GLFWwindow* getGLFWWindow() const;
    bool shouldClose() const;
    void pollEvents();
    
    // Callbacks
    void setResizeCallback(std::function<void(int, int)> callback);
    void setKeyCallback(std::function<void(int, int, int, int)> callback);
    void setMouseButtonCallback(std::function<void(int, int, int)> callback);
    
private:
    GLFWwindow* glfwWindow;
    int width, height;
};
```

**Tasks:**
1. Wrap GLFW window management
2. Set up GLFW callbacks
3. Create Application class with lifecycle
4. Input manager (keyboard, mouse state)
5. Delta time tracking
6. Basic camera controls (WASD, mouse orbit)

**Deliverable:** Clean application structure, camera controls work

---

### Day 10-11: Renderer Architecture
**Goal:** Separate rendering concerns

**Files to create:**
```
src/render/Renderer.h
src/render/Renderer.cpp
src/render/RenderPass.h
src/render/Buffer.h
src/render/Buffer.cpp
```

**Renderer class:**
```cpp
class Renderer {
public:
    void init(VulkanContext* context, Window* window);
    void beginFrame();
    void endFrame();
    void shutdown();
    
    VkCommandBuffer getCurrentCommandBuffer();
    uint32_t getCurrentFrame() const;
    
private:
    void createCommandBuffers();
    void createSyncObjects();
    
    VulkanContext* context;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    
    uint32_t currentFrame = 0;
    const int MAX_FRAMES_IN_FLIGHT = 2;
};
```

**Tasks:**
1. Abstract command buffer management
2. Handle frame synchronization
3. Multi-frame in-flight rendering
4. Proper resource cleanup
5. Resize handling

**Deliverable:** Solid rendering foundation

---

### Day 12-14: Project Organization & Build System
**Goal:** Clean, scalable structure

**Files to update:**
```
CMakeLists.txt (improved)
.gitignore
README.md
docs/BUILD.md
```

**Directory structure:**
```
VulkanProject/
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── Application.h/cpp
│   │   ├── Window.h/cpp
│   │   └── Input.h/cpp
│   ├── render/
│   │   ├── VulkanContext.h/cpp
│   │   ├── SwapChain.h/cpp
│   │   ├── GraphicsPipeline.h/cpp
│   │   ├── Renderer.h/cpp
│   │   ├── Shader.h/cpp
│   │   └── Buffer.h/cpp
│   └── ui/ (empty for now)
├── shaders/
│   ├── basic.vert
│   ├── basic.frag
│   └── compile.bat (Windows shader compiler script)
├── assets/
│   └── fonts/ (empty for now)
├── build/ (gitignored)
├── CMakeLists.txt
├── .gitignore
└── README.md
```

**CMakeLists.txt improvements:**
```cmake
# Organize sources
file(GLOB_RECURSE CORE_SOURCES "src/core/*.cpp")
file(GLOB_RECURSE RENDER_SOURCES "src/render/*.cpp")
file(GLOB_RECURSE UI_SOURCES "src/ui/*.cpp")

set(ALL_SOURCES
    src/main.cpp
    ${CORE_SOURCES}
    ${RENDER_SOURCES}
    ${UI_SOURCES}
)

# Shader compilation
find_program(GLSLC glslc HINTS $ENV{VULKAN_SDK}/Bin)

file(GLOB SHADER_SOURCES 
    "shaders/*.vert" 
    "shaders/*.frag"
)

foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(SHADER_OUTPUT "${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME}.spv")
    
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/shaders/"
        COMMAND ${GLSLC} ${SHADER} -o ${SHADER_OUTPUT}
        DEPENDS ${SHADER}
    )
    
    list(APPEND SPIRV_SHADERS ${SHADER_OUTPUT})
endforeach()

add_custom_target(Shaders DEPENDS ${SPIRV_SHADERS})
add_dependencies(VulkanProject Shaders)
```

**Tasks:**
1. Organize code into modules
2. Automate shader compilation
3. Write build documentation
4. Set up proper include paths
5. Add logging system (spdlog)

**Deliverable:** Professional project structure

---

**END OF PHASE 1 CHECKPOINT:**
- ✅ Vulkan rendering works
- ✅ Can render 3D geometry
- ✅ Camera controls functional
- ✅ Clean architecture
- ✅ Ready to build UI on top

---

# PHASE 2: CORE UI SYSTEM (Weeks 3-6)

## Week 3: UI Architecture & Event System

### Day 15-16: Widget Base Class
**Goal:** Foundation for all UI elements

**Files to create:**
```
src/ui/Widget.h
src/ui/Widget.cpp
src/ui/UITypes.h
src/ui/Event.h
```

**UITypes.h:**
```cpp
#pragma once
#include <glm/glm.hpp>

struct Rect {
    float x, y, width, height;
    
    bool contains(glm::vec2 point) const;
    glm::vec2 center() const;
    glm::vec2 topLeft() const;
    glm::vec2 bottomRight() const;
};

struct Color {
    float r, g, b, a;
    
    static Color fromRGB(uint8_t r, uint8_t g, uint8_t b);
    static Color fromHex(uint32_t hex);
};

enum class Alignment {
    TopLeft, Top, TopRight,
    Left, Center, Right,
    BottomLeft, Bottom, BottomRight
};

struct Padding {
    float top, right, bottom, left;
    
    Padding(float all) : top(all), right(all), bottom(all), left(all) {}
    Padding(float vertical, float horizontal) 
        : top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}
};
```

**Event.h:**
```cpp
#pragma once
#include <glm/glm.hpp>

enum class EventType {
    None,
    MouseDown, MouseUp, MouseMove,
    KeyDown, KeyUp,
    WindowResize, WindowClose,
    Custom
};

enum class MouseButton {
    Left, Right, Middle
};

struct Event {
    EventType type;
    bool handled = false;
};

struct MouseEvent : Event {
    glm::vec2 position;
    MouseButton button;
    int clickCount = 1;
};

struct KeyEvent : Event {
    int key;
    int scancode;
    int mods;
};
```

**Widget.h:**
```cpp
#pragma once
#include "UITypes.h"
#include "Event.h"
#include <vector>
#include <memory>
#include <functional>

class UIRenderer; // Forward declaration

class Widget {
public:
    Widget();
    virtual ~Widget();
    
    // Hierarchy
    void addChild(Widget* child);
    void removeChild(Widget* child);
    Widget* getParent() const { return parent; }
    const std::vector<Widget*>& getChildren() const { return children; }
    
    // Properties
    void setBounds(const Rect& bounds);
    Rect getBounds() const { return bounds; }
    Rect getAbsoluteBounds() const;
    
    void setVisible(bool visible) { this->visible = visible; }
    bool isVisible() const { return visible; }
    
    void setEnabled(bool enabled) { this->enabled = enabled; }
    bool isEnabled() const { return enabled; }
    
    // Layout
    virtual void layout();
    void setNeedsLayout() { needsLayout = true; }
    
    // Rendering
    virtual void paint(UIRenderer* renderer);
    void setNeedsRepaint() { needsRepaint = true; }
    
    // Events
    virtual void onMouseDown(const MouseEvent& e) {}
    virtual void onMouseUp(const MouseEvent& e) {}
    virtual void onMouseMove(const MouseEvent& e) {}
    virtual void onKeyDown(const KeyEvent& e) {}
    virtual void onKeyUp(const KeyEvent& e) {}
    
    // Hit testing
    virtual bool hitTest(glm::vec2 point) const;
    Widget* findWidgetAt(glm::vec2 point);
    
protected:
    Rect bounds;
    Widget* parent = nullptr;
    std::vector<Widget*> children;
    
    bool visible = true;
    bool enabled = true;
    bool needsLayout = true;
    bool needsRepaint = true;
    
    // State
    bool hovered = false;
    bool pressed = false;
    bool focused = false;
};
```

**Tasks:**
1. Implement Widget base class
2. Tree structure (parent/child relationships)
3. Bounds and transforms (local → absolute)
4. Visibility and enabled state
5. Hit testing (which widget is under mouse)
6. Event virtual functions

**Deliverable:** Widget base class with hierarchy

---

### Day 17-18: Event Manager
**Goal:** Route events to widgets

**Files to create:**
```
src/ui/EventManager.h
src/ui/EventManager.cpp
```

**EventManager.h:**
```cpp
class EventManager {
public:
    EventManager(Widget* rootWidget);
    
    // Process GLFW callbacks
    void handleMouseButton(int button, int action, int mods);
    void handleMouseMove(double xpos, double ypos);
    void handleKey(int key, int scancode, int action, int mods);
    
    // Focus management
    void setFocusedWidget(Widget* widget);
    Widget* getFocusedWidget() const { return focusedWidget; }
    
private:
    void updateHoverState(glm::vec2 mousePos);
    void dispatchEvent(Event& event);
    
    Widget* rootWidget;
    Widget* hoveredWidget = nullptr;
    Widget* focusedWidget = nullptr;
    Widget* pressedWidget = nullptr;
    
    glm::vec2 lastMousePos;
};
```

**Tasks:**
1. Convert GLFW events to UI events
2. Route events to correct widget (hit testing)
3. Track hovered widget
4. Track pressed widget (drag tracking)
5. Focus management
6. Event propagation (capture/bubble)

**Deliverable:** Events reach correct widgets

---

### Day 19-21: UI Renderer (2D Vulkan Pipeline)
**Goal:** Render UI primitives

**Files to create:**
```
src/ui/UIRenderer.h
src/ui/UIRenderer.cpp
shaders/ui.vert
shaders/ui.frag
```

**UIRenderer.h:**
```cpp
class UIRenderer {
public:
    void init(VulkanContext* context, VkRenderPass renderPass);
    void shutdown();
    
    void begin(VkCommandBuffer commandBuffer, glm::vec2 viewportSize);
    void end();
    
    // Drawing primitives
    void drawRect(const Rect& rect, const Color& color);
    void drawRectOutline(const Rect& rect, const Color& color, float thickness);
    void drawRoundedRect(const Rect& rect, float radius, const Color& color);
    void drawLine(glm::vec2 start, glm::vec2 end, const Color& color, float thickness);
    void drawTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, const Color& color);
    
    // Text rendering (placeholder for now)
    void drawText(const std::string& text, glm::vec2 position, const Color& color);
    
    // Clipping
    void pushClipRect(const Rect& rect);
    void popClipRect();
    
private:
    struct UIVertex {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::vec4 color;
    };
    
    void createPipeline(VkRenderPass renderPass);
    void createBuffers();
    void updateBuffers();
    
    VulkanContext* context;
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    
    std::vector<UIVertex> vertices;
    std::vector<uint32_t> indices;
    
    Buffer* vertexBuffer;
    Buffer* indexBuffer;
    
    glm::mat4 projectionMatrix;
    std::vector<Rect> clipStack;
};
```

**UI Shaders:**
```glsl
// ui.vert
#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragUV;

layout(push_constant) uniform PushConstants {
    mat4 projection;
} push;

void main() {
    gl_Position = push.projection * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
    fragUV = inUV;
}

// ui.frag
#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = fragColor;
}
```

**Tasks:**
1. Create 2D Vulkan pipeline (orthographic projection)
2. Vertex format for UI (position, UV, color)
3. Dynamic vertex/index buffers
4. Draw primitives (rect, line, etc.)
5. Scissor/clipping support
6. Push constants for projection matrix

**Deliverable:** Can render colored rectangles

---

## Week 4: Basic Widgets

### Day 22-23: Panel Widget
**Goal:** Container widget

**Files to create:**
```
src/ui/widgets/Panel.h
src/ui/widgets/Panel.cpp
```

**Panel.h:**
```cpp
class Panel : public Widget {
public:
    Panel();
    
    void setBackgroundColor(const Color& color);
    void setBorderColor(const Color& color);
    void setBorderWidth(float width);
    void setPadding(const Padding& padding);
    
    void paint(UIRenderer* renderer) override;
    void layout() override;
    
private:
    Color backgroundColor = Color{0.2f, 0.2f, 0.2f, 1.0f};
    Color borderColor = Color{0.4f, 0.4f, 0.4f, 1.0f};
    float borderWidth = 1.0f;
    Padding padding = Padding(5.0f);
};
```

**Tasks:**
1. Implement Panel as Widget subclass
2. Draw background color
3. Draw border
4. Apply padding to children
5. Layout children (simple for now)

**Deliverable:** Panel widget renders

---

### Day 24-25: Label Widget
**Goal:** Display text (placeholder implementation)

**Files to create:**
```
src/ui/widgets/Label.h
src/ui/widgets/Label.cpp
```

**Label.h:**
```cpp
class Label : public Widget {
public:
    Label(const std::string& text = "");
    
    void setText(const std::string& text);
    std::string getText() const { return text; }
    
    void setTextColor(const Color& color);
    void setFontSize(float size);
    void setAlignment(Alignment align);
    
    void paint(UIRenderer* renderer) override;
    
private:
    std::string text;
    Color textColor = Color{1.0f, 1.0f, 1.0f, 1.0f};
    float fontSize = 16.0f;
    Alignment alignment = Alignment::Left;
};
```

**Tasks:**
1. Store text string
2. Placeholder text rendering (simple box for now)
3. Text color property
4. Alignment property
5. Calculate text bounds (estimate for now)

**Note:** Real text rendering comes in Week 5

**Deliverable:** Label widget (shows placeholder box)

---

### Day 26-28: Button Widget
**Goal:** Interactive button

**Files to create:**
```
src/ui/widgets/Button.h
src/ui/widgets/Button.cpp
```

**Button.h:**
```cpp
class Button : public Widget {
public:
    Button(const std::string& text = "");
    
    void setText(const std::string& text);
    void setOnClick(std::function<void()> callback);
    
    void paint(UIRenderer* renderer) override;
    void onMouseDown(const MouseEvent& e) override;
    void onMouseUp(const MouseEvent& e) override;
    void onMouseMove(const MouseEvent& e) override;
    
private:
    std::string text;
    std::function<void()> onClickCallback;
    
    Color normalColor = Color{0.3f, 0.3f, 0.3f, 1.0f};
    Color hoverColor = Color{0.4f, 0.4f, 0.4f, 1.0f};
    Color pressedColor = Color{0.2f, 0.2f, 0.2f, 1.0f};
    Color textColor = Color{1.0f, 1.0f, 1.0f, 1.0f};
};
```

**Tasks:**
1. Visual states (normal, hover, pressed, disabled)
2. Mouse event handling
3. onClick callback
4. State-based color changes
5. Draw button background and text

**Deliverable:** Working button with click detection

---

## Week 5-6: Text Rendering

### Day 29-31: FreeType Integration
**Goal:** Render real text

**Install FreeType:**
```bash
cd C:\vcpkg
.\vcpkg install freetype:x64-windows
```

**Files to create:**
```
src/ui/text/Font.h
src/ui/text/Font.cpp
src/ui/text/TextRenderer.h
src/ui/text/TextRenderer.cpp
```

**Font.h:**
```cpp
struct Glyph {
    uint32_t textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    uint32_t advance;
};

class Font {
public:
    Font(const std::string& filepath, float size);
    ~Font();
    
    const Glyph* getGlyph(char c) const;
    float getLineHeight() const;
    glm::vec2 measureText(const std::string& text) const;
    
private:
    void loadGlyphs();
    
    FT_Face face;
    float size;
    std::unordered_map<char, Glyph> glyphs;
    uint32_t atlasTexture; // Font atlas
};
```

**TextRenderer.h:**
```cpp
class TextRenderer {
public:
    void init(VulkanContext* context, VkRenderPass renderPass);
    void shutdown();
    
    void drawText(VkCommandBuffer cmd, const std::string& text, 
                  glm::vec2 position, Font* font, const Color& color);
    
private:
    void createTextPipeline();
    void createFontAtlas(Font* font);
    
    VulkanContext* context;
    VkPipeline textPipeline;
    VkPipelineLayout textPipelineLayout;
    VkDescriptorSetLayout descriptorSetLayout;
    
    // Font atlas texture
    VkImage fontAtlasImage;
    VkImageView fontAtlasView;
    VkSampler fontSampler;
};
```

**Text Shaders:**
```glsl
// text.vert
#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragUV;

layout(push_constant) uniform PushConstants {
    mat4 projection;
} push;

void main() {
    gl_Position = push.projection * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
    fragUV = inUV;
}

// text.frag
#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragUV;

layout(binding = 0) uniform sampler2D fontAtlas;

layout(location = 0) out vec4 outColor;

void main() {
    float alpha = texture(fontAtlas, fragUV).r;
    outColor = vec4(fragColor.rgb, fragColor.a * alpha);
}
```

**Tasks:**
1. Initialize FreeType library
2. Load TrueType font
3. Generate glyph atlas (texture with all characters)
4. Render glyphs to texture
5. Create text rendering pipeline
6. Implement drawText function
7. Text measurement (width, height)
8. Support multiple fonts

**Deliverable:** Real text rendering works!

---

### Day 32-35: Text Layout & Features
**Goal:** Advanced text handling

**Tasks:**
1. Text wrapping (word wrap)
2. Text alignment (left, center, right)
3. Multi-line text
4. Text clipping
5. UTF-8 support (basic)
6. Text selection (visual, for later input)

**Update Label and Button:**
- Use real text rendering
- Proper text measurement
- Auto-sizing based on text

**Deliverable:** Professional text rendering

---

**END OF PHASE 2 CHECKPOINT:**
- ✅ Widget system works
- ✅ Event routing functional
- ✅ Can render UI primitives
- ✅ Basic widgets (Panel, Button, Label)
- ✅ Real text rendering
- ✅ Ready for complex widgets

---

# PHASE 3: ESSENTIAL WIDGETS (Weeks 7-9)

## Week 7: Input Widgets

### Day 36-38: TextInput Widget
**Goal:** User text input

**Files to create:**
```
src/ui/widgets/TextInput.h
src/ui/widgets/TextInput.cpp
```

**TextInput.h:**
```cpp
class TextInput : public Widget {
public:
    TextInput();
    
    void setText(const std::string& text);
    std::string getText() const { return text; }
    
    void setPlaceholder(const std::string& placeholder);
    void setOnTextChanged(std::function<void(const std::string&)> callback);
    
    void paint(UIRenderer* renderer) override;
    void onMouseDown(const MouseEvent& e) override;
    void onKeyDown(const KeyEvent& e) override;
    
private:
    std::string text;
    std::string placeholder;
    size_t cursorPosition = 0;
    
    bool isFocused = false;
    float cursorBlinkTime = 0.0f;
    
    std::function<void(const std::string&)> onTextChangedCallback;
};
```

**Tasks:**
1. Text editing (insert, delete, backspace)
2. Cursor rendering and movement
3. Cursor blinking animation
4. Text selection (drag to select)
5. Copy/paste support (GLFW clipboard)
6. Focus handling (click to focus)
7. Keyboard shortcuts (Ctrl+A, Ctrl+C, Ctrl+V)

**Deliverable:** Working text input field

---

### Day 39-40: Checkbox Widget

**Files to create:**
```
src/ui/widgets/Checkbox.h
src/ui/widgets/Checkbox.cpp
```

**Checkbox.h:**
```cpp
class Checkbox : public Widget {
public:
    Checkbox(const std::string& label = "");
    
    void setChecked(bool checked);
    bool isChecked() const { return checked; }
    
    void setOnChanged(std::function<void(bool)> callback);
    
    void paint(UIRenderer* renderer) override;
    void onMouseDown(const MouseEvent& e) override;
    
private:
    bool checked = false;
    std::string label;
    std::function<void(bool)> onChangedCallback;
};
```

**Tasks:**
1. Toggle state on click
2. Draw checkbox box
3. Draw checkmark when checked
4. Label text next to checkbox
5. Hover effect

**Deliverable:** Working checkbox

---

### Day 41-42: Slider Widget

**Files to create:**
```
src/ui/widgets/Slider.h
src/ui/widgets/Slider.cpp
```

**Slider.h:**
```cpp
class Slider : public Widget {
public:
    Slider(float min = 0.0f, float max = 1.0f);
    
    void setValue(float value);
    float getValue() const { return value; }
    
    void setRange(float min, float max);
    void setOnValueChanged(std::function<void(float)> callback);
    
    void paint(UIRenderer* renderer) override;
    void onMouseDown(const MouseEvent& e) override;
    void onMouseMove(const MouseEvent& e) override;
    void onMouseUp(const MouseEvent& e) override;
    
private:
    float value = 0.0f;
    float minValue = 0.0f;
    float maxValue = 1.0f;
    bool dragging = false;
    
    std::function<void(float)> onValueChangedCallback;
};
```

**Tasks:**
1. Draw track (background)
2. Draw thumb (handle)
3. Drag to change value
4. Click track to jump to position
5. Value constraints (min/max)
6. Visual feedback (hover, pressed)

**Deliverable:** Working slider

---

## Week 8: Container Widgets

### Day 43-45: ScrollArea Widget
**Goal:** Scrollable content

**Files to create:**
```
src/ui/widgets/ScrollArea.h
src/ui/widgets/ScrollArea.cpp
```

**ScrollArea.h:**
```cpp
class ScrollArea : public Widget {
public:
    ScrollArea();
    
    void setContent(Widget* content);
    Widget* getContent() const { return content; }
    
    void scrollTo(float x, float y);
    glm::vec2 getScrollOffset() const;
    
    void paint(UIRenderer* renderer) override;
    void layout() override;
    void onMouseMove(const MouseEvent& e) override;
    
private:
    Widget* content = nullptr;
    glm::vec2 scrollOffset = {0, 0};
    glm::vec2 contentSize = {0, 0};
    
    bool showScrollbars = true;
    bool draggingScrollbar = false;
};
```

**Tasks:**
1. Content clipping (scissor test)
2. Scroll offset management
3. Mouse wheel scrolling
4. Scrollbar rendering
5. Scrollbar dragging
6. Calculate content size
7. Layout content with offset

**Deliverable:** Scrollable area with scrollbars

---

### Day 46-47: List Widget

**Files to create:**
```
src/ui/widgets/List.h
src/ui/widgets/List.cpp
```

**List.h:**
```cpp
class List : public Widget {
public:
    List();
    
    void addItem(const std::string& text);
    void removeItem(size_t index);
    void clear();
    
    void setSelectedIndex(int index);
    int getSelectedIndex() const { return selectedIndex; }
    
    void setOnSelectionChanged(std::function<void(int)> callback);
    
    void paint(UIRenderer* renderer) override;
    void layout() override;
    void onMouseDown(const MouseEvent& e) override;
    
private:
    std::vector<std::string> items;
    int selectedIndex = -1;
    int hoveredIndex = -1;
    
    float itemHeight = 25.0f;
    
    std::function<void(int)> onSelectionChangedCallback;
};
```

**Tasks:**
1. Store list items
2. Render items
3. Selection (single select)
4. Hover effect
5. Click to select
6. Scroll integration (if needed)
7. Item height calculation

**Deliverable:** Working list widget

---

### Day 48-49: Dropdown/ComboBox Widget

**Files to create:**
```
src/ui/widgets/Dropdown.h
src/ui/widgets/Dropdown.cpp
```

**Tasks:**
1. Closed state (shows selected item)
2. Open state (shows list)
3. Click to open/close
4. Item selection
5. Position dropdown list (overlay)
6. Close on outside click

**Deliverable:** Working dropdown

---

## Week 9: Advanced Widgets

### Day 50-52: ColorPicker Widget

**Files to create:**
```
src/ui/widgets/ColorPicker.h
src/ui/widgets/ColorPicker.cpp
```

**Tasks:**
1. Color square (saturation/value)
2. Hue slider
3. Alpha slider (optional)
4. Color preview
5. Hex input field
6. RGB sliders (optional)
7. Drag to pick color

**Deliverable:** Working color picker

---

### Day 53-54: Menu System

**Files to create:**
```
src/ui/widgets/Menu.h
src/ui/widgets/Menu.cpp
src/ui/widgets/MenuItem.h
src/ui/widgets/MenuBar.h
```

**MenuBar.h:**
```cpp
class MenuBar : public Widget {
public:
    MenuBar();
    
    Menu* addMenu(const std::string& title);
    
    void paint(UIRenderer* renderer) override;
    void layout() override;
    
private:
    std::vector<Menu*> menus;
};
```

**Menu.h:**
```cpp
class Menu : public Widget {
public:
    Menu(const std::string& title);
    
    MenuItem* addItem(const std::string& text, std::function<void()> callback);
    void addSeparator();
    Menu* addSubmenu(const std::string& text);
    
    void open();
    void close();
    
    void paint(UIRenderer* renderer) override;
    
private:
    std::string title;
    std::vector<MenuItem*> items;
    bool isOpen = false;
};
```

**Tasks:**
1. Menu bar at top
2. Menu items
3. Click to open menu
4. Submenu support
5. Keyboard shortcuts display
6. Separator lines
7. Close on item click or outside click

**Deliverable:** Working menu system

---

**END OF PHASE 3 CHECKPOINT:**
- ✅ Complete widget library
- ✅ Text input, sliders, checkboxes
- ✅ Lists, dropdowns, color picker
- ✅ Menu system
- ✅ Ready for layout and theming

---

# PHASE 4: LAYOUT & WINDOWS (Weeks 10-12)

## Week 10: Layout System

### Day 55-57: Layout Managers

**Files to create:**
```
src/ui/layout/Layout.h
src/ui/layout/BoxLayout.h
src/ui/layout/BoxLayout.cpp
src/ui/layout/GridLayout.h
src/ui/layout/GridLayout.cpp
```

**Layout.h:**
```cpp
class Layout {
public:
    virtual ~Layout() = default;
    virtual void layout(Widget* container) = 0;
};
```

**BoxLayout.h:**
```cpp
enum class Orientation {
    Horizontal,
    Vertical
};

class BoxLayout : public Layout {
public:
    BoxLayout(Orientation orientation);
    
    void setSpacing(float spacing);
    void setMargin(const Padding& margin);
    
    void layout(Widget* container) override;
    
private:
    Orientation orientation;
    float spacing = 5.0f;
    Padding margin = Padding(5.0f);
};
```

**GridLayout.h:**
```cpp
class GridLayout : public Layout {
public:
    GridLayout(int rows, int columns);
    
    void setSpacing(float spacing);
    void setCellSize(glm::vec2 size);
    
    void layout(Widget* container) override;
    
private:
    int rows, columns;
    float spacing = 5.0f;
    glm::vec2 cellSize = {0, 0}; // Auto if 0
};
```

**Tasks:**
1. HBox (horizontal layout)
2. VBox (vertical layout)
3. Grid layout
4. Spacing between widgets
5. Margin/padding
6. Widget size constraints (min/max/preferred)
7. Stretch factors (widget takes remaining space)

**Deliverable:** Automatic layout system

---

### Day 58-60: Constraint-Based Layout

**Files to create:**
```
src/ui/layout/Constraints.h
src/ui/layout/Constraints.cpp
```

**Constraints.h:**
```cpp
enum class SizePolicy {
    Fixed,      // Use specified size
    Expanding,  // Take all available space
    Minimum,    // Use minimum required size
    Preferred   // Use preferred size if available
};

struct SizeConstraint {
    float min = 0;
    float max = FLT_MAX;
    float preferred = 0;
    SizePolicy policy = SizePolicy::Preferred;
};

class Widget {
    // Add to Widget class:
    void setMinSize(glm::vec2 size);
    void setMaxSize(glm::vec2 size);
    void setPreferredSize(glm::vec2 size);
    void setSizePolicy(SizePolicy horizontal, SizePolicy vertical);
    
    virtual glm::vec2 calculateMinSize() const;
    virtual glm::vec2 calculatePreferredSize() const;
};
```

**Tasks:**
1. Size policies (fixed, expanding, minimum)
2. Min/max size constraints
3. Preferred size calculation
4. Widget size negotiation
5. Layout constraint solving

**Deliverable:** Flexible sizing system

---

### Day 61-63: Anchors & Alignment

**Files to create:**
```
src/ui/layout/Anchors.h
```

**Tasks:**
1. Anchor to parent edges (top, left, bottom, right)
2. Center anchoring
3. Relative positioning
4. Percentage-based sizing
5. Aspect ratio constraints

**Deliverable:** Advanced positioning options

---

## Week 11: Window Management

### Day 64-66: Window Widget

**Files to create:**
```
src/ui/widgets/Window.h
src/ui/widgets/Window.cpp
```

**Window.h:**
```cpp
class Window : public Panel {
public:
    Window(const std::string& title);
    
    void setTitle(const std::string& title);
    void setMoveable(bool moveable);
    void setResizable(bool resizable);
    void setCloseable(bool closeable);
    
    void close();
    
    void paint(UIRenderer* renderer) override;
    void layout() override;
    void onMouseDown(const MouseEvent& e) override;
    void onMouseMove(const MouseEvent& e) override;
    void onMouseUp(const MouseEvent& e) override;
    
private:
    std::string title;
    
    bool moveable = true;
    bool resizable = true;
    bool closeable = true;
    
    bool dragging = false;
    bool resizing = false;
    glm::vec2 dragOffset;
    
    // Title bar
    Rect titleBarRect;
    Button* closeButton;
    
    // Resize handles
    enum class ResizeHandle {
        None, Top, Bottom, Left, Right,
        TopLeft, TopRight, BottomLeft, BottomRight
    };
    ResizeHandle activeResizeHandle = ResizeHandle::None;
};
```

**Tasks:**
1. Title bar with text
2. Close button
3. Move window (drag title bar)
4. Resize handles (8 directions)
5. Resize logic
6. Minimum window size
7. Bring to front on click
8. Window decorations

**Deliverable:** Moveable, resizable windows

---

### Day 67-69: Docking System (Basic)

**Files to create:**
```
src/ui/docking/DockSpace.h
src/ui/docking/DockSpace.cpp
```

**DockSpace.h:**
```cpp
enum class DockSide {
    None, Left, Right, Top, Bottom, Center
};

class DockSpace : public Widget {
public:
    DockSpace();
    
    void dockWindow(Window* window, DockSide side);
    void undockWindow(Window* window);
    
    void paint(UIRenderer* renderer) override;
    void layout() override;
    
private:
    struct DockNode {
        Widget* content = nullptr;
        DockNode* parent = nullptr;
        DockNode* left = nullptr;
        DockNode* right = nullptr;
        float splitRatio = 0.5f;
        bool isHorizontalSplit = false;
    };
    
    DockNode* rootNode;
    std::vector<Window*> floatingWindows;
};
```

**Tasks:**
1. Dock space widget
2. Dock preview (highlight where window will dock)
3. Split dock space
4. Tab groups (multiple windows in one dock)
5. Drag to dock/undock
6. Splitter bars (resize docked panels)

**Note:** This is complex - implement basic version first

**Deliverable:** Basic docking functionality

---

## Week 12: Modal Dialogs & Popups

### Day 70-72: Modal Dialog System

**Files to create:**
```
src/ui/widgets/Dialog.h
src/ui/widgets/Dialog.cpp
src/ui/widgets/MessageBox.h
```

**Dialog.h:**
```cpp
class Dialog : public Window {
public:
    Dialog(const std::string& title);
    
    enum class Result {
        None, OK, Cancel, Yes, No
    };
    
    void show();
    void showModal();  // Blocks other UI
    Result getResult() const { return result; }
    
protected:
    Result result = Result::None;
    bool isModal = false;
};
```

**MessageBox.h:**
```cpp
class MessageBox : public Dialog {
public:
    enum class Type {
        Information,
        Warning,
        Error,
        Question
    };
    
    enum class Buttons {
        OK,
        OKCancel,
        YesNo,
        YesNoCancel
    };
    
    MessageBox(const std::string& title, const std::string& message, 
               Type type = Type::Information,
               Buttons buttons = Buttons::OK);
    
    static Result show(const std::string& title, const std::string& message,
                      Type type = Type::Information,
                      Buttons buttons = Buttons::OK);
};
```

**Tasks:**
1. Modal overlay (dim background)
2. Block input to other windows
3. Message box (OK, OK/Cancel, Yes/No)
4. File dialog (basic)
5. Dialog result handling
6. ESC to close

**Deliverable:** Modal dialogs work

---

### Day 73-75: Popup & Tooltip System

**Files to create:**
```
src/ui/widgets/Popup.h
src/ui/widgets/Tooltip.h
```

**Tasks:**
1. Popup widget (context menu style)
2. Tooltip on hover (delayed)
3. Positioning (avoid screen edges)
4. Auto-close on outside click
5. Fade in/out animation

**Deliverable:** Popups and tooltips

---

**END OF PHASE 4 CHECKPOINT:**
- ✅ Layout system works
- ✅ Windows can be moved/resized
- ✅ Basic docking
- ✅ Modal dialogs
- ✅ Complete window management
- ✅ Ready for theming and polish

---

# PHASE 5: ADVANCED FEATURES (Weeks 13-16)

## Week 13-14: Theme System

### Day 76-78: Theme Engine

**Files to create:**
```
src/ui/theme/Theme.h
src/ui/theme/Theme.cpp
src/ui/theme/StyleSheet.h
```

**Theme.h:**
```cpp
struct WidgetStyle {
    // Colors
    Color backgroundColor;
    Color foregroundColor;
    Color borderColor;
    Color textColor;
    
    // States
    Color hoverBackgroundColor;
    Color pressedBackgroundColor;
    Color disabledBackgroundColor;
    
    // Sizing
    float borderWidth;
    float borderRadius;
    Padding padding;
    
    // Text
    float fontSize;
    std::string fontFamily;
};

class Theme {
public:
    static Theme* getCurrent();
    static void setCurrent(Theme* theme);
    
    WidgetStyle getStyle(const std::string& widgetType) const;
    void setStyle(const std::string& widgetType, const WidgetStyle& style);
    
    // Predefined themes
    static Theme* createDarkTheme();
    static Theme* createLightTheme();
    static Theme* createBlenderTheme();
    
private:
    std::unordered_map<std::string, WidgetStyle> styles;
    
    static Theme* currentTheme;
};
```

**Tasks:**
1. Theme class with widget styles
2. Color scheme definition
3. Runtime theme switching
4. Widget queries theme for colors
5. Multiple built-in themes:
   - Dark theme
   - Light theme
   - Blender-inspired theme
6. Theme serialization (save/load JSON)

**Deliverable:** Theme system with multiple themes

---

### Day 79-81: Style Customization

**Tasks:**
1. Per-widget style overrides
2. Style inheritance (children inherit parent styles)
3. Dynamic style updates (immediate repaint)
4. Style properties:
   - Colors (background, foreground, border, text)
   - Sizes (border width, border radius, padding)
   - Fonts (family, size, weight)
5. State-based styling (normal, hover, pressed, disabled, focused)

**Deliverable:** Fully customizable styling

---

### Day 82-84: Icon System

**Files to create:**
```
src/ui/icons/Icon.h
src/ui/icons/IconFont.h
```

**Tasks:**
1. Icon font loading (FontAwesome style)
2. Icon rendering
3. Icon + text in buttons
4. Icon-only buttons
5. Icon library
6. Color icons (tinted)

**Deliverable:** Icon support in UI

---

## Week 15: Animation System

### Day 85-87: Animation Framework

**Files to create:**
```
src/ui/animation/Animation.h
src/ui/animation/Animation.cpp
src/ui/animation/Tween.h
```

**Animation.h:**
```cpp
enum class EasingFunction {
    Linear,
    EaseIn, EaseOut, EaseInOut,
    BounceIn, BounceOut,
    ElasticIn, ElasticOut
};

template<typename T>
class Animation {
public:
    Animation(T startValue, T endValue, float duration, EasingFunction easing);
    
    void start();
    void stop();
    void pause();
    void update(float deltaTime);
    
    T getCurrentValue() const;
    bool isFinished() const;
    
    void setOnComplete(std::function<void()> callback);
    
private:
    T startValue, endValue, currentValue;
    float duration;
    float elapsedTime = 0.0f;
    EasingFunction easing;
    bool playing = false;
};
```

**Tasks:**
1. Animation class (generic)
2. Easing functions (linear, ease-in/out, bounce, elastic)
3. Update animations each frame
4. Animate properties:
   - Position (slide)
   - Size (grow/shrink)
   - Color (fade)
   - Opacity (fade in/out)
5. Animation callbacks (onComplete)
6. Animation manager (track all active animations)

**Deliverable:** Animation framework

---

### Day 88-90: Widget Animations

**Tasks:**
1. Fade in/out animations
2. Slide animations (windows, panels)
3. Hover effects (color transitions)
4. Button press animations
5. Window open/close animations
6. Tooltip fade-in with delay
7. Smooth scrolling

**Update widgets to use animations:**
- Button color transitions
- Window slide in
- Dialog fade in with overlay
- Tooltip delayed fade
- Menu slide down

**Deliverable:** Animated UI elements

---

## Week 16: Performance & Polish

### Day 91-93: Performance Optimization

**Tasks:**
1. **Dirty Rectangles:** Only repaint changed areas
2. **Widget Culling:** Don't paint invisible widgets
3. **Batch Rendering:** Minimize draw calls
4. **Vertex Buffer Optimization:** Reuse buffers
5. **Layout Caching:** Only re-layout when needed
6. **Event Optimization:** Early exit hit testing
7. **Memory Pooling:** Reduce allocations
8. **Profiling:** Measure frame time, identify bottlenecks

**Optimizations:**
```cpp
class Widget {
    bool needsRepaint = true;
    bool needsLayout = true;
    Rect dirtyRect;  // Only this area needs repainting
    
    void markDirty(const Rect& rect);
    bool isInViewport() const;  // Culling
};

class UIRenderer {
    void beginFrame();  // Start batch
    void flushBatch();  // Upload and draw
    
    std::vector<UIVertex> batchVertices;  // Accumulate
};
```

**Deliverable:** 60fps UI with thousands of widgets

---

### Day 94-96: Accessibility & Polish

**Tasks:**
1. **Keyboard Navigation:**
   - Tab to navigate widgets
   - Enter to activate
   - Arrow keys for lists
   - Focus indicators

2. **Accessibility:**
   - Screen reader support (ARIA-like)
   - High contrast themes
   - Scalable fonts
   - Keyboard-only operation

3. **Polish:**
   - Smooth animations
   - Consistent spacing
   - Proper z-ordering
   - No visual glitches
   - Responsive (no lag)

4. **Debug Tools:**
   - Widget inspector
   - Layout visualizer
   - Performance overlay (FPS, draw calls)
   - Event logger

**Deliverable:** Professional, accessible UI

---

**END OF PHASE 5 CHECKPOINT:**
- ✅ Theme system complete
- ✅ Animations working
- ✅ Performance optimized
- ✅ Professional polish
- ✅ Production-ready UI system

---

# PHASE 6: INTEGRATION & REAL APPLICATION (Weeks 17-18)

## Week 17: Application Integration

### Day 97-98: Main Application UI

**Files to create:**
```
src/app/MainWindow.h
src/app/MainWindow.cpp
src/app/Toolbar.h
src/app/Viewport3D.h
```

**MainWindow layout:**
```
┌─────────────────────────────────────┐
│ Menu Bar (File, Edit, View, Help)  │
├─────────────────────────────────────┤
│ Toolbar (Icons for common actions)  │
├──────┬──────────────────────┬───────┤
│      │                      │       │
│ Left │   3D Viewport        │ Right │
│Panel │   (Your Vulkan 3D)   │ Panel │
│      │                      │       │
│      │                      │       │
├──────┴──────────────────────┴───────┤
│ Status Bar                          │
└─────────────────────────────────────┘
```

**Tasks:**
1. Create main window layout
2. Menu bar with File, Edit, View, Help
3. Toolbar with common actions
4. 3D viewport widget (embeds your Vulkan rendering)
5. Side panels (properties, tools)
6. Status bar (info, progress)
7. Connect UI to 3D rendering

**Deliverable:** Complete application layout

---

### Day 99-100: Tool Panels & Properties

**Tasks:**
1. Properties panel (object properties)
2. Tool panel (mode switcher)
3. Outliner/hierarchy panel
4. Settings dialog
5. Connect panels to 3D scene
6. Update UI from scene changes

**Example:**
```cpp
class PropertiesPanel : public Panel {
public:
    void setSelectedObject(Object* obj) {
        // Show object properties in UI
        positionX->setValue(obj->position.x);
        positionY->setValue(obj->position.y);
        // ...
    }
};
```

**Deliverable:** Functional tool panels

---

### Day 101-102: Preferences & Settings

**Tasks:**
1. Preferences dialog
2. Theme selection
3. Keyboard shortcuts customization
4. Viewport settings
5. Performance settings
6. Save/load preferences (JSON)

**Deliverable:** Complete preferences system

---

## Week 18: Testing & Documentation

### Day 103-104: Testing

**Tasks:**
1. Test all widgets
2. Test layout system
3. Test window management
4. Test keyboard navigation
5. Test on different resolutions
6. Fix bugs
7. Polish rough edges

**Deliverable:** Bug-free UI

---

### Day 105-107: Documentation

**Tasks:**
1. **API Documentation:**
   - Widget class reference
   - How to create custom widgets
   - Layout system guide
   - Theme customization guide

2. **User Guide:**
   - How to use the application
   - Keyboard shortcuts
   - Tips and tricks

3. **Code Examples:**
   - Creating custom widgets
   - Using layouts
   - Handling events
   - Theming

**Deliverable:** Complete documentation

---

### Day 108-110: Example Applications

**Create demo applications:**

1. **UI Showcase:**
   - Demonstrates all widgets
   - Theme switcher
   - Interactive examples

2. **Simple Editor:**
   - File browser
   - Text editor with syntax highlighting
   - Shows docking, windows, menus

3. **Properties Inspector:**
   - Like Blender's properties panel
   - Multiple tabs
   - Complex forms

**Deliverable:** Demo applications

---

**END OF PHASE 6 - PROJECT COMPLETE:**
- ✅ Full retained-mode UI system
- ✅ Complete widget library
- ✅ Layout system
- ✅ Window management
- ✅ Theming & animations
- ✅ Integrated with 3D application
- ✅ Production-ready
- ✅ Documented

---

# Summary: Complete File Structure

```
VulkanProject/
├── src/
│   ├── main.cpp
│   │
│   ├── core/
│   │   ├── Application.h/cpp
│   │   ├── Window.h/cpp
│   │   └── Input.h/cpp
│   │
│   ├── render/
│   │   ├── VulkanContext.h/cpp
│   │   ├── SwapChain.h/cpp
│   │   ├── GraphicsPipeline.h/cpp
│   │   ├── Renderer.h/cpp
│   │   ├── Shader.h/cpp
│   │   └── Buffer.h/cpp
│   │
│   ├── ui/
│   │   ├── Widget.h/cpp
│   │   ├── UITypes.h
│   │   ├── Event.h
│   │   ├── EventManager.h/cpp
│   │   ├── UIRenderer.h/cpp
│   │   │
│   │   ├── widgets/
│   │   │   ├── Panel.h/cpp
│   │   │   ├── Label.h/cpp
│   │   │   ├── Button.h/cpp
│   │   │   ├── TextInput.h/cpp
│   │   │   ├── Checkbox.h/cpp
│   │   │   ├── Slider.h/cpp
│   │   │   ├── ScrollArea.h/cpp
│   │   │   ├── List.h/cpp
│   │   │   ├── Dropdown.h/cpp
│   │   │   ├── ColorPicker.h/cpp
│   │   │   ├── Menu.h/cpp
│   │   │   ├── Window.h/cpp
│   │   │   ├── Dialog.h/cpp
│   │   │   ├── MessageBox.h/cpp
│   │   │   ├── Popup.h/cpp
│   │   │   └── Tooltip.h/cpp
│   │   │
│   │   ├── layout/
│   │   │   ├── Layout.h
│   │   │   ├── BoxLayout.h/cpp
│   │   │   ├── GridLayout.h/cpp
│   │   │   ├── Constraints.h/cpp
│   │   │   └── Anchors.h
│   │   │
│   │   ├── text/
│   │   │   ├── Font.h/cpp
│   │   │   └── TextRenderer.h/cpp
│   │   │
│   │   ├── theme/
│   │   │   ├── Theme.h/cpp
│   │   │   └── StyleSheet.h
│   │   │
│   │   ├── animation/
│   │   │   ├── Animation.h/cpp
│   │   │   └── Tween.h
│   │   │
│   │   ├── docking/
│   │   │   └── DockSpace.h/cpp
│   │   │
│   │   └── icons/
│   │       ├── Icon.h
│   │       └── IconFont.h
│   │
│   └── app/
│       ├── MainWindow.h/cpp
│       ├── Toolbar.h/cpp
│       ├── Viewport3D.h/cpp
│       └── PropertiesPanel.h/cpp
│
├── shaders/
│   ├── basic.vert/frag (3D)
│   ├── ui.vert/frag (2D UI)
│   ├── text.vert/frag (Text)
│   └── compile.bat
│
├── assets/
│   ├── fonts/
│   │   └── Roboto-Regular.ttf
│   └── icons/
│       └── icon_font.ttf
│
├── build/ (ignored)
├── CMakeLists.txt
├── .gitignore
├── README.md
└── docs/
    ├── BUILD.md
    ├── API.md
    └── USER_GUIDE.md
```

---

# Key Milestones

**Week 2:** Triangle renders, camera works  
**Week 4:** First button clicks  
**Week 6:** Real text renders  
**Week 9:** All basic widgets done  
**Week 12:** Windows can dock  
**Week 16:** Animations smooth, themes work  
**Week 18:** Complete application

---

# Next Steps

**Are you ready to start Phase 1, Week 1, Day 1?**

We'll begin with:
1. Initialize Vulkan instance
2. Select physical device
3. Create logical device
4. Set up validation layers

**Should we start building?** 🚀
