# ==========================================
# Compiler & Flags Configurations
# ==========================================
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g

mingw64  := "C:/Program Files/MSYS2/mingw64"
# ==========================================
# Directory Structures
# ==========================================
CODE_DIR    := ../code
CORE_DIR    := $(CODE_DIR)/core
TESTS_DIR   := $(CODE_DIR)/tests_core
SHADERS_DIR := ../$(CODE_DIR)/shaders
IMGUI_DIR   := $(CODE_DIR)/vendor/imgui
BUILD_DIR   := ../build

# ==========================================
# Include Directories
# ==========================================
INCLUDES := -I$(CODE_DIR) -I$(CORE_DIR) $(shell find $(CORE_DIR) -type d | sed 's/^/-I/') -I$(mingw64)/include -I$(SHADERS_DIR) -I$(IMGUI_DIR)

TARGET      := $(BUILD_DIR)/app.exe
TEST_TARGET := $(BUILD_DIR)/run_tests.exe

# ==========================================
# Source Files Discovery
# ==========================================
# קובצי ה-cpp של הקוד הראשי
CORE_SRCS := $(shell find $(CORE_DIR) -type f -name "*.cpp")

# קבצי הליבה וה-Backends הנדרשים בלבד של ImGui (מונע טעינת backends מיותרים כמו allegro)
IMGUI_SRCS := $(IMGUI_DIR)/imgui.cpp \
              $(IMGUI_DIR)/imgui_draw.cpp \
              $(IMGUI_DIR)/imgui_tables.cpp \
              $(IMGUI_DIR)/imgui_widgets.cpp \
              $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
              $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

# קובץ ה-main הראשי של התוכנית (עבור ה-Target הרגיל)
APP_MAIN := $(CORE_DIR)/main.cpp

# קובצי הקוד ללא main.cpp (עבור ה-Tests)
LIB_CORE_SRCS := $(filter-out $(APP_MAIN), $(CORE_SRCS))

# קובצי ה-cpp של הטסטים
TEST_SRCS := $(shell find $(TESTS_DIR) -type f -name "*.cpp")

# ==========================================
# Object & Dependency Files
# ==========================================
CORE_OBJS   := $(patsubst $(CORE_DIR)/%.cpp, $(BUILD_DIR)/core/%.o, $(CORE_SRCS))
IMGUI_OBJS  := $(patsubst $(IMGUI_DIR)/%.cpp, $(BUILD_DIR)/imgui/%.o, $(IMGUI_SRCS))
LIB_CORE_OBJS := $(patsubst $(CORE_DIR)/%.cpp, $(BUILD_DIR)/core/%.o, $(LIB_CORE_SRCS))
TEST_OBJS   := $(patsubst $(TESTS_DIR)/%.cpp, $(BUILD_DIR)/tests/%.o, $(TEST_SRCS))

APP_OBJS  := $(CORE_OBJS) $(IMGUI_OBJS)
LIB_OBJS  := $(LIB_CORE_OBJS) $(IMGUI_OBJS)

DEPS := $(APP_OBJS:.o=.d) $(TEST_OBJS:.o=.d)

# ==========================================
# External Libraries
# ==========================================
GTEST_LIBS := -lgtest -lgtest_main -lpthread
GLFW_LIBS  := -L"C:/Program Files/MSYS2/mingw64/lib/" -lglfw3 -lglew32 -lopengl32 -lgdi32 -mconsole

# ==========================================
# Rules
# ==========================================

.PHONY: all clean run test

# ברירת מחדל: בניית האפליקציה הראשית והטסטים
all: $(TARGET) $(TEST_TARGET)

# בניית התוכנית הראשית
$(TARGET): $(APP_OBJS)
	@echo "Linking executable: $@"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(APP_OBJS) -o $@ $(GLFW_LIBS)
	@cp "C:/Program Files/MSYS2/mingw64/bin/glfw3.dll" $(dir $@)
	@cp "C:/Program Files/MSYS2/mingw64/bin/glew32.dll" $(dir $@)
	@echo "Build successful!"

# בניית קובץ הריצה של הטסטים
$(TEST_TARGET): $(LIB_OBJS) $(TEST_OBJS)
	@echo "Linking test executable: $@"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LIB_OBJS) $(TEST_OBJS) -o $@ $(GTEST_LIBS) $(GLFW_LIBS)
	@echo "Test build successful!"

# קימפול קובצי קוד מקור (core/)
$(BUILD_DIR)/core/%.o: $(CORE_DIR)/%.cpp
	@echo "Compiling Code: $< -> $@"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# קימפול קובצי ImGui (vendor/imgui/)
$(BUILD_DIR)/imgui/%.o: $(IMGUI_DIR)/%.cpp
	@echo "Compiling ImGui: $< -> $@"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -Wno-unused-parameter -MMD -MP -c $< -o $@

# קימפול קובצי בדיקות (tests/)
$(BUILD_DIR)/tests/%.o: $(TESTS_DIR)/%.cpp
	@echo "Compiling Test: $< -> $@"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# הכללת קובצי תלויות למניעת קימפול מחדש של מה שלא השתנה
-include $(DEPS)

# הרצת האפליקציה הראשית
run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

# הרצת בדיקות היחידה
test: $(TEST_TARGET)
	@echo "Running Tests..."
	@./$(TEST_TARGET)

# ניקוי תיקיית הבנייה
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)
	@echo "Clean complete."
