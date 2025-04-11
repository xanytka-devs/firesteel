![Banner](docs/banner.png "Firesteel banner")
# Firesteel
[![Discord](https://img.shields.io/discord/635140721908908049?style=flat&logo=discord&logoColor=fff)](https://discord.gg/jw6YSwTFrU)
[![Static Badge](https://img.shields.io/badge/chat-Telegram-blue?logo=telegram&logoColor=white)](https://t.me/xanytka)
[![Build status](https://ci.appveyor.com/api/projects/status/9pq8id42s9mgxfoi?svg=true)](https://ci.appveyor.com/project/sanyaalabai/firesteel)
[![Documentation Status](https://readthedocs.org/projects/Firesteel/badge/?version=latest)](https://firesteel.readthedocs.io/ru/latest/?badge=latest)
[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)  
Понятный, кроссплатформенный и полностью настраиваемый движок для создания игр и программ.

## Возмодности создания игр
На данный момент движок работает как програмный слой (имеет редактор с конфигурацией).
## Возмодности создания программ
На данный момент можно использовать лишь ImGUI, который включён в билд, но ведётся работа над нативной UI библиотекой (fs.ui).
## Пакетная система
Весь функционал движка поставляется через пакеты. Автоматически включаются только отрисовщик тестового GUI, загрузчики контента и мат. база. Дополнительно пакеты можно загружать с [сайта пакетов](https://xanytka.ru/p/).

# Политика публикации
Игры и программы можно свободно публиковать в любом интернет-магазине ([XGLauncher](https://github.com/xanytka-devs/xglauncher), [Steam](https://store.steampowered.com/), [Epic Games Store](https://store.epicgames.com/) и т.д.).
При издании физических копий игры/программы необходимо [сделать запрос к Xanytka](https://support.xanytka.ru/publisher/phys_copies).

# Сторонние библиотеки
* [Assimp](https://github.com/assimp/assimp)
* [Box2D](https://github.com/erincatto/box2d)
* [Bullet3](https://github.com/bulletphysics/bullet3.git)
* [GLM](https://github.com/g-truc/glm)
* [ImGUI](https://github.com/ocornut/imgui)
* [Nlohmann JSON](https://github.com/nlohmann/json)
* [OpenAL Soft](https://github.com/kcat/openal-soft)
* [STB](https://github.com/nothings/stb)

# Установка
## Windows
* Запустить команду `git clone --recursive https://github.com/xanytka-devs/firesteel.git engine`
* Установить [CMake](https://cmake.org/)
* Создать файл `main.cpp`, в котором вы будете писать код приложения
* Создать файл `CMakeLists.txt` с содержимым:
```cmake
cmake_minimum_required(VERSION 3.12)
project(example)
add_subdirectory(engine)

add_executable(example
	"main.cpp"
)
target_link_libraries(example
	firesteel
	glad
	imgui
	glm
	assimp
	OpenAL
)
target_compile_features(example PUBLIC cxx_std_17)
target_include_directories(example PUBLIC includes)
target_include_directories(example PUBLIC engine/external/glfw)
set_target_properties(example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
```
* Постройте проект через CMake
	* Создайте папку `build`
	* Откройте командую строку в этой новой папке и введите в неё `cmake ..`

> [!NOTE]
> Установка для Linux и MacOS ещё не продумана, но сильно отличаться не должна.
