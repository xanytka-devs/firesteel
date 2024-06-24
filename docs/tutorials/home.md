# Создание первого проекта
Для начала разработки вам необходим пустой проект. Давайте разберёмся во вариантах его создания:
## Менеджер проектов
На данный момент самый простой вариант создания проектов вне готовых конфигураций.  
Всё, что нужно сделать - запустить **prj_manager.cmd**. В нём вы должны указать название проекта и выбрать, будете ли вы использовать сборку через **CMake**.
## Вручную
Надо хардкора? Если нет, то идите дальше: ([след. пункт](https://firesteel.readthedocs.io/ru/latest/tutorials/triangle.html)).  
Если же не шутить, то это очень просто.
1. Создаём папку для проекта 
Если вы используете **CMake**, то лучше создавайте папку прямо в папке с клонировнным репозиторием Firesteel.
2. В папке проекта создаём файл **main.cpp**
3. Если вы используете **CMake**  
Создайте в папке вашего проекта файл **CMakeLists.txt** с содержимым:
``` cmake
cmake_minimum_required(VERSION 3.12)
project(название_проекта)
set(PROJECT_NAME название_проекта)
set(PRJ_ALL
	main.cpp
)

add_executable(${PROJECT_NAME}
	${PRJ_ALL}
)
target_link_libraries(${PROJECT_NAME} firesteel)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC includes)
target_include_directories(${PROJECT_NAME} PRIVATE src)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
```
После этого сохраните файл. Теперь перейдите в корневую папку Firesteel и в лежащий там **CMakeLists.txt** добавьте в конец файла:
``` cmake
add_subdirectory(название_проекта)
```
Сохраните файл и сконфигурируйте обстановку используя **CMake (cmake-gui)** или **AutoCMake.cmd** в той же папке.  
4. Если вы не используете **CMake**  
*Пункт не завершён. Потычьте кабана по имени DoggySasha чтобы он дописал.*

# Заключение
На этом данная статья закончена. Как улучшение комфорта использования Wiki в каждой следующей статье в конце документа будет две кнопки (одна на статью назад, другая - вперёд).  
[<- На страницу firesteel](https://github.com/xanytka-devs/firesteel) | [Первый треугольник ->](https://firesteel.readthedocs.io/ru/latest/tutorials/triangle.html)