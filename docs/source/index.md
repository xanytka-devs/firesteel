![Banner](https://github.com/xanytka-devs/firesteel/blob/main/docs/assets/banner.png?raw=true "Firesteel banner")

# Добро пожаловать в Firesteel!
Данная Wiki поможет вам преодолеть все трудности, которые могут возникнуть при разработке на программном слое firesteel.  
Если вам нужен именно редактор среды (конфигурация с подготовленным интерфейсом и сборкой), то используйте [FS Editor](https://xanytka.ru/shared/fse).

# Создание первого проекта
Для начала разработки вам необходим пустой проект. Давайте разберёмся в его создании:
<!--## Менеджер проектов
На данный момент самый простой вариант создания проектов вне готовых конфигураций.  
Всё, что нужно сделать - запустить **prj_manager.cmd**. В нём вы должны указать название проекта и выбрать, будете ли вы использовать сборку через **CMake**.
## Вручную
Надо хардкора? Если нет, то идите дальше: ([след. пункт](https://firesteel.readthedocs.io/ru/latest/#tutorials/your-own-window/)).  
Если же не шутить, то это очень просто.-->
1. Создаём папку для проекта 
Если вы используете **CMake**, то лучше создавайте папку прямо в папке с клонировнным репозиторием Firesteel.
2. Запустите команду `git clone --recursive https://github.com/xanytka-devs/firesteel.git engine`
3. Установите [CMake](https://cmake.org/)
4. Создайте файл `main.cpp`, в котором вы будете писать код приложения
5. Создайте файл `CMakeLists.txt` с содержимым:
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
)
target_compile_features(example PUBLIC cxx_std_17)
target_include_directories(example PUBLIC includes)
target_include_directories(example PUBLIC engine/external/glfw)
set_target_properties(example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
```
6. Постройте проект через CMake
	* Создайте папку `build`
	* Откройте командую строку в этой новой папке и введите в неё `cmake ..`