TEMPLATE = app
CONFIG -= qt
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL
INCLUDEPATH +=
QMAKE_CXXFLAGS += -std=c++11
SOURCES += \
    main.cpp \
    Game.cpp \
    tools.cpp \
    Scene.cpp \
    graphics/Texture.cpp \
    graphics/TextureManager.cpp \
    graphics/Shader.cpp \
    graphics/ShaderProgram.cpp \
    audio/AudioManager.cpp \
    audio/SoundEffect.cpp \
    audio/Music.cpp \
    input/InputManager.cpp \
    SceneMain/SceneMain.cpp \
    SceneMain/GameObject.cpp \
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

HEADERS += \
    Scene.hpp \
    Game.hpp \
    tools.hpp \
    RenderState.hpp \
    graphics/Texture.hpp \
    graphics/TextureManager.hpp \
    graphics/Shader.hpp \
    graphics/ShaderProgram.hpp \
    audio/AudioManager.hpp \
    audio/SoundEffect.hpp \
    audio/Music.hpp \
    input/InputManager.hpp \
    SceneMain/SceneMain.hpp \
    SceneMain/GameObject.hpp \
    glm/*.hpp \
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

OTHER_FILES += \
    data/shaders/*.vert \
    data/shaders/*.frag \
    data/anim/*.anim

