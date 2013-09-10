TEMPLATE = app
CONFIG -= qt
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL
INCLUDEPATH += $$PWD/
QMAKE_CXXFLAGS += -std=c++11
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/Game.cpp \
    $$PWD/tools.cpp \
    $$PWD/Scene.cpp \
    $$PWD/graphics/Texture.cpp \
    $$PWD/graphics/TextureManager.cpp \
    $$PWD/graphics/Shader.cpp \
    $$PWD/graphics/ShaderProgram.cpp \
    $$PWD/audio/AudioManager.cpp \
    $$PWD/audio/SoundEffect.cpp \
    $$PWD/audio/Music.cpp \
    $$PWD/input/InputManager.cpp \
    $$PWD/SceneMain/SceneMain.cpp \
    $$PWD/SceneMain/GameObject.cpp \
    SceneMain/TriangleObject.cpp \
    graphics/Mesh.cpp \
    graphics/Vertex.cpp \
    graphics/Model.cpp \
    graphics/Uniform.cpp \
    graphics/ShaderBinding.cpp \
    SceneMain/RegularPolygonObject.cpp \
    SceneMain/Map.cpp \
    SceneMain/Player.cpp \
    SceneMain/Character.cpp \
    SceneMain/InputEngine.cpp \
    SceneMain/Person.cpp \
    SceneMain/Npc.cpp \
    SceneMain/House.cpp \
    SceneMain/Animation.cpp \
    Utils.cpp
    SceneMain/House.cpp

HEADERS += \
    $$PWD/Scene.hpp \
    $$PWD/Game.hpp \
    $$PWD/tools.hpp \
    $$PWD/RenderState.hpp \
    $$PWD/graphics/Texture.hpp \
    $$PWD/graphics/TextureManager.hpp \
    $$PWD/graphics/Shader.hpp \
    $$PWD/graphics/ShaderProgram.hpp \
    $$PWD/audio/AudioManager.hpp \
    $$PWD/audio/SoundEffect.hpp \
    $$PWD/audio/Music.hpp \
    $$PWD/input/InputManager.hpp \
    $$PWD/SceneMain/SceneMain.hpp \
    $$PWD/SceneMain/GameObject.hpp \
    $$PWD/glm/*.hpp \
    SceneMain/TriangleObject.hpp \
    graphics/Mesh.hpp \
    graphics/Vertex.hpp \
    graphics/Model.hpp \
    graphics/Uniform.hpp \
    graphics/ShaderBinding.hpp \
    SceneMain/RegularPolygonObject.hpp \
    SceneMain/Map.hpp \
    SceneMain/Player.hpp \
    SceneMain/Character.hpp \
    SceneMain/InputEngine.hpp \
    SceneMain/Person.hpp \
    SceneMain/Npc.hpp \
    SceneMain/House.hpp \
    SceneMain/Animation.hpp \
    Utils.hpp
    SceneMain/House.hpp

OTHER_FILES += \
    $$PWD/shaders/sample.vert \
    $$PWD/shaders/sample.frag \
    shaders/sample2.vert \
    shaders/sample2.frag \
    shaders/house.frag \
    shaders/house.vert \
    data/shaders/house.vert \
    data/shaders/house.frag \
    data/shaders/tex.frag \
    data/shaders/tex.vert \
    data/shaders/color.frag \
    data/shaders/color.vert
