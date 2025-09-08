# Отрисовка модели без загрузки
Может это поможет вам сделать воксельную игру или какую-либо нестандартную геометрию.
Перед выполнением данного туториала рекомендуется всё же [попробывать загрузить модель](https://firesteel.readthedocs.io/ru/latest/tutorials/loading_models/), чтобы иметь базовое понимание о моделях в Firesteel.

## Пояснение перед началом
Начнём с главного - все модели состоят из **мешей** и **нодов**.
Ноды нам пока не нужны. Главное знать, что они позволяют *трансформировать* меши.
А вот меши являются сеткой и состоят они из треугольников, а именно полигонов. На данный момент Firesteel не поддерживает ничего кроме полигонов по стандарту.  
Так вот полигоны состоят из двух самых важных компонентов - **точек углов** и **индексов**.  
![Полигон](https://github.com/xanytka-devs/firesteel/blob/model_loaders/docs/assets/custom_mesh0.png?raw=true)  
Первые определяют все возможные точки, которые позже объединяются в треугольники и отрисовываются на экран.
Вторые же позволяют сократить итоговое количество точек. Они позволяют переиспользывать уже существующие точки.  
К примеру у куба 6 сторон. А у каждой стороны по 2 треугольника внутри. И у каждого треугольника по 3 точки.
Тем самым 6\*2\*3=36 точек! Но зачем их 36, если можно использовать уже готовые и сократить их итоговое необходимое количество до 8?
Для этого как раз и нужны **индексы**.

# Треугольник (полигон)
Почему бы не отрисовать сам полигон?  
Введите следущий код в `main.cpp`:
``` cpp
#include <../include/firesteel.hpp>
using namespace Firesteel;

Entity entity;

class CustomMesh : public Firesteel::App {
    virtual void onInitialize() override {
        //Setup mesh.
        const float setup[3][3]={
            { 1,     1,    0},
            { 0,    -1,    0},
            {-1,     1,    1}
        };
        std::vector<Vertex> verticies(3);
        std::vector<unsigned int> indicies{0,1,2};
        for(unsigned int i=0;i<3;i++) {
            Vertex vert{};
            vert.position=glm::vec3(setup[i][0],setup[i][1],setup[i][2]);
            vert.normal=glm::vec3(i==0,i==1,i==2);
            verticies[i]=vert;
        }
        //Append mesh to the model.
        entity.addMesh(verticies,indicies,&material);
    }
    virtual void onUpdate() override {
        entity.draw();
    }
    virtual void onShutdown() override {
        entity.remove();
        material.remove();
    }
};

int main() {
    return CustomMesh{}.start();
}
```
В данном коде составляется равнобедренный треугольник. На данный момент **Firesteel** налегает на пространственные кооринаты окна, предоставляемые **OpenGL**, а именно: левая нижняя сторона это (-1,-1), правая верхняя это (1,1).

## Промежуточный результат \#1
![Кромешная тьма](https://github.com/xanytka-devs/firesteel/blob/main/docs/assets/your-own-window.png?raw=true)  
Чтож... Что-то пошло не так. Ну начнём с того, что полигон повёрнут на 180 градусов от камеры. Для лучшей производительности в Firesteel задние стороны полигонов не отрисовываются - поэтому его и не видно.  
А также нету материала.

# Крутим треугольник
``` cpp
#include <../include/firesteel.hpp>
using namespace Firesteel;

Material material;
Entity entity;

class CustomMesh : public Firesteel::App {
    virtual void onInitialize() override {
        material.setShader("shader.vs", "shader.fs");
        //Setup mesh.
        const float setup[3][3]={
            { 1,     1,    0},
            { 0,    -1,    0},
            {-1,     1,    1}
        };
        std::vector<Vertex> verticies(3);
        std::vector<unsigned int> indicies{0,1,2};
        for(unsigned int i=0;i<3;i++) {
            Vertex vert{};
            vert.position=glm::vec3(setup[i][0],setup[i][1],setup[i][2]);
            verticies[i]=vert;
        }
        //Append mesh to the model.
        entity.addMesh(verticies,indicies,&material);
        //Adjust model to be visible.
        entity.transform.rotation.x=180.f;
        entity.transform.size=glm::vec3(0.8f);
    }
    virtual void onUpdate() override {
        entity.draw();
    }
    virtual void onShutdown() override {
        entity.remove();
        material.remove();
    }
};

int main() {
    return CustomMesh{}.start();
}
```
Теперь треугольник повёрнут и чуть меньше по размеру. Заодно добавим шейдеры:

## Вертексный шейдер
``` glsl
#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 frag_POS;

uniform mat4 model;

void main() {
   gl_Position = model*vec4(aPos, 1.0);
   frag_POS = aPos;
}
```

## Фрагментный шейдер
``` glsl
#version 330 core
out vec4 frag_COLOR;
in vec3 frag_POS;

void main() {
    frag_COLOR = vec4((frag_POS/2+vec3(0.5))-frag_POS,1);
}
```
Ну тут не самое красивое решение, однако оно всё равно предоставляет нужный результат. Просто передать вектор не получилось бы, так как у полигона были положения с отрицательными координатами.

# Промежуточный результат \#2
![RGB треугольник](https://github.com/xanytka-devs/firesteel/blob/model_loaders/docs/assets/custom_mesh1.png?raw=true)  
Ну теперь можно отрисовать и пластину (квад).

# Отрисовываем пластину
Пластину передать чуть сложнее чем треугольник. Как вы знаете у треугольника три точки и Firesteel может только их и отрисовывать. Так что надо как-то 4 точки превратить в треугольники. Визуально это должно выглядить так:  
![Пластина из двух треугольников](https://github.com/xanytka-devs/firesteel/blob/model_loaders/docs/assets/custom_mesh2.png?raw=true)  
Однако если не использовать индексы и просто передать точки для двух треугольников, то выйдет вот так:  
![Неоптимизированная пластина](https://github.com/xanytka-devs/firesteel/blob/model_loaders/docs/assets/custom_mesh4.png?raw=true)  
Ну это никуда не пойдёт. Зачем создавать 6 точек, если можно справиться всего 4-мя? Используя индексы можно сказать Firesteel-у, что первый треугольник стоит на точках 1, 4 и 3, а второй на 1, 2 и 3.  
![Оптимизированная пластина](https://github.com/xanytka-devs/firesteel/blob/model_loaders/docs/assets/custom_mesh3.png?raw=true)  
Теперь надо написать для этого код.

## Код для отрисовки пластины
Внесите в `main.cpp` следущие изменения:
``` cpp
#include <../include/firesteel.hpp>
using namespace Firesteel;

Material material;
Entity entity;

class CustomMesh : public Firesteel::App {
    virtual void onInitialize() override {
        material.setShader("shader.vs", "shader.fs");
        //Setup mesh.
        const float setup[4][3]={
            {-1,    -1,    0},
            {-1,     1,    0.5f},
            { 1,    -1,    0.5f},
            { 1,     1,    1}
        };
        std::vector<Vertex> verticies(4);
        std::vector<unsigned int> indicies{0,3,2,1,3,0};
        for(unsigned int i=0;i<4;i++) {
            Vertex vert{};
            vert.position=glm::vec3(setup[i][0],setup[i][1],setup[i][2]);
            verticies[i]=vert;
        }
        //Append mesh to the model.
        entity.addMesh(verticies,indicies,&material);
        //Adjust model to be visible.
        entity.transform.rotation.x=180.f;
        entity.transform.size=glm::vec3(0.8f);
    }
    virtual void onUpdate() override {
        entity.draw();
    }
    virtual void onShutdown() override {
        entity.remove();
        material.remove();
    }
};

int main() {
    return CustomMesh{}.start();
}
```

# Промежуточный результат \#3
![RGB пластина (почти)](https://github.com/xanytka-devs/firesteel/blob/model_loaders/docs/assets/custom_mesh5.png?raw=true)  
На этом собственно всё.