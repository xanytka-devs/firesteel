# 🔼 Предусловие
Начнём с простого - "Hello World!" на **Firesteel**.

# Отрисовка окна
Для создания простого окна нужно [скачать пакет](https://firesteel.readthedocs.io/ru/latest/#tutorials/packets/) "fs.rendering.opengl" (или любой другой для отображения) в файл **main.cpp** добавить несколько простых строчек кода:
``` cpp
#include <firesteel/app.hpp>
using namespace firesteel;
class HelloWorldApp : public App { };

int main() {
    //Create instance of app.
    auto app = new HelloWorldApp();
    return app->start(800, 600, "Hello World!");
}
```

# Рисуем треугольник
Теперь, так как у нас окно, то давайте добавим в него простой треугольник.
## Перед классом **HelloWorldApp** добавим:
``` cpp
/// <summary>
/// Triangle example class.
/// </summary>
class Triangle : public Transform {
public:
    //Constructor.
    Triangle() : Transform(glm::vec3(0.f), glm::vec4(glm::vec3(0.f), 1.f), glm::vec3(1.f)) {}

    //Initialization method.
    void initialize() {
        //Initialize mesh.
        unsigned int vert_num = 3;
        float vertices[] = {
            //       POSITIONS                 NORMAL               UVs
                -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
                 0.5f, -0.5f,  0.0f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
                 0.0f,  0.5f,  0.0f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f
        };
        //Enumerate indicies.
        std::vector<unsigned int> indicies(vert_num);
        for (unsigned int i = 0; i < vert_num; i++)
            indicies[i] = i;
        //Add mesh to render list.
        m_meshes.push_back(Mesh(Vertex::generate_list(vertices, vert_num), indicies));
        //Initialize base class.
        Transform::initialize();
    }

    //Rendering method.
    void render() {
        //Just call base class.
        Transform::render();
    }
};
```

## После этого проинициализируем класс треугольника и материала для него.
``` cpp
Triangle triangle{};
Material mat;
```

## Теперь добавим несколько изменений в приложение.
``` cpp
class HelloWorldApp : public App {
    virtual void initialize() override {
        //Initialize material.
        mat.shader = Shader("res\\core.vert", "res\\core.frag");
        triangle.set_material(&mat);
        //Initialize triangle.
        triangle.initialize();
    }

    virtual void update() override {
        //Enable shader.
        mat.shader.enable();
        //Set position variable.
        mat.shader.set_3_floats("aPos", triangle.position);
        //Draw triangle.
        triangle.render();
    }

    virtual void on_shutdown() override {
        //Clean up shader.
        mat.remove();
        triangle.remove();
    }
};
```

## Завершим всё это тем, что добавим шейдер для треугольника.
Построим проект (не запустим). В папке с построенным проектом создадим папку **res** и добавим в неё файлы **core.vert** и **core.frag**.
### Содержимое core.vert:
``` c
#version 330 core
in vec3 aPos;
out vec3 pos;

void main() {
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   pos = aPos;
}
```
### Содержимое core.frag:
``` c
#version 330 core
out vec4 FragColor;
in vec3 pos;

void main() {
    FragColor = vec4(pos.x, pos.y, pos.z, 1.0f);
}
```
Теперь треугольник будет иметь градиент из красного, синего и зелёного.

# Заключение
После всех этих действий вы будете иметь окно с треугольником, какой обычно создают как "Hello World" на OpenGL или других графических библиотеках.  
Итоговый код: [https://github.com/xanytka-devs/fs-examples/tree/main/triangle_demo](https://github.com/xanytka-devs/fs-examples/tree/main/triangle_demo)  
[<- Пакеты](https://firesteel.readthedocs.io/ru/latest/#tutorials/packages/) | [Продолжение... ->](https://firesteel.readthedocs.io/ru/latest/#tutorials/home/)