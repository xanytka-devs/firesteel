![Banner](docs/assets/banner.png)  
[![Build status](https://ci.appveyor.com/api/projects/status/9pq8id42s9mgxfoi?svg=true)](https://ci.appveyor.com/project/sanyaalabai/firesteel)
[![Documentation Status](https://img.shields.io/readthedocs/firesteel?logo=readthedocs&logoColor=ccc)](https://firesteel.readthedocs.io/ru/latest/?badge=latest)
[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)
[![Static Badge](https://img.shields.io/badge/chat-Telegram-orange?logo=telegram&logoColor=ccc)](https://t.me/xanytka)  
# Firesteel
Понятный, кроссплатформенный\* и полностью настраиваемый программный слой для создания игр и программ.

## Возможности создания игр
Программный слой имеет лишь загрузчики контента, отрисовщик и математическую базу. Однако также имеется обширная библиотека [пакетов](https://xanytka.ru/p).
Если же вам необходимо более простое решение, то рекомендуется использование [FSE](https://xanytka.ru/shared/fse).

## Возможности создания программ
Программный слой использует ImGUI, который включён в билд, но ведётся работа над нативной UI библиотекой [fs.ui](https://xanytka.ru/p/search/?search=fs.ui&tags=&env=none).

## Пакетная система
Весь функционал движка поставляется через пакеты. Дополнительные пакеты можно загружать с [общего репозитория](https://xanytka.ru/p/).

# Политика публикации
Игры и программы можно свободно публиковать в любом интернет-магазине ([Steam](https://store.steampowered.com/), [Epic Games Store](https://store.epicgames.com/) и т.д.).
При издании физических копий игры/программы необходимо [сделать запрос к Xanytka](https://support.xanytka.ru/publisher/phys_copies).

# Сторонние библиотеки
* [GLM](https://github.com/g-truc/glm)
* [ImGUI](https://github.com/ocornut/imgui)
* [Nlohmann JSON](https://github.com/nlohmann/json)
* [STB](https://github.com/nothings/stb)
* [TinyGLTF](https://github.com/syoyo/tinygltf)
* [TinyOBJLoader](https://github.com/tinyobjloader/tinyobjloader)
* [OpenFBX](https://github.com/nem0/OpenFBX)

# Установка

## Упрощённая
* Запустите команду
``` bash
git clone --recursive https://github.com/xanytka-devs/fs-example.git
```
* Установите [CMake](https://cmake.org/)
* Запустите `auto_cmake.cmd`

## Обычная
* Запустите команду 
``` bash
git submodule add --recursive https://github.com/xanytka-devs/firesteel.git engine
```
* Установите [CMake](https://cmake.org/)
* Создайте файл `main.cpp`, в котором вы будете писать код приложения
* Создайте файл `CMakeLists.txt` с содержимым:
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
> Установка и работа на Linux и MacOS ещё не реализованы, однако некоторые компоненты уже могут работать с Linux.
