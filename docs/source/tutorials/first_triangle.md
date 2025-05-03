> [!WARNING]
> Данная статья находится в этапе переписания.

# Предисловие
Сделаем немного нестандартное, но полезное упражнение.

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
``` glsl
#version 330 core
in vec3 aPos;
out vec3 pos;

void main() {
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   pos = aPos;
}
```
### Содержимое core.frag:
``` glsl
#version 330 core
out vec4 FragColor;
in vec3 pos;

void main() {
    FragColor = vec4(pos.x, pos.y, pos.z, 1.0f);
}
```
Теперь треугольник будет иметь градиент из красного, синего и зелёного.

# Заключение
Итоговый код: https://github.com/xanytka-devs/fs-examples/tree/main/triangle_demo  
[<- Своё окно](https://firesteel.readthedocs.io/ru/latest/#tutorials/your-own-window/) | [Продолжение... ->](https://github.com/xanytka-devs/firesteel/wiki)