# Отрисовка модели
Для начала загрузим и отрисуем модель. Если вы хотите идти рука-об-руку с туториалом, то модель рюкзака, а также текстуры к нему можно скачать [здесь](https://github.com/xanytka-devs/fs-examples/blob/main/res/ModelLoading/).
Напишите следующий код в `main.cpp`:
``` cpp
#include "engine/include/firesteel.hpp"
using namespace Firesteel;

Entity entity;

class ModelLoaderApp : public Firesteel::App {
    virtual void onInitialize() override {
        entity.load("backpack.obj");
    }
    virtual void onUpdate() override {
        entity.draw();
    }
    virtual void onShutdown() override {
        entity.remove();
    }
};

int main() {
    return ModelLoaderApp{}.start();
}
```
После постройки и запуска проекта вы увидите модель рюкзака, но при этом она будет почти на весь экран и не будет иметь текстур:
![Оранжевая модель на весь экран](https://github.com/xanytka-devs/firesteel/blob/main/docs/assets/loading_models0.png?raw=true)
Почему так? Ну во-первых к модели не применяется никакого материала, из-за этого она использует **fallback** (аварийный ресурс).

# Свой материал
Полноценные материалы в данном случае не нужны, так как текстуры не изменяются. Просто создадим шейдер и заменим им стандартный.
Существует несколько этапов отрисовки модели. В данном туториале будут затронуты лишь две стадии: Вертексная и Фрагментная.  
Первая нужна для обработки положений и данных треугольников, а вторая отвечает за отрисовку пикселей этих треугольников на экран.
## Вертексный шейдер
Создайте файл `shader.vs` и добавьте в него следущий код:
``` glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;

out vec3 frag_POS;
out vec2 frag_UV;

uniform mat4 model;

void main() {
	frag_POS = vec3(model * vec4(aPos, 1.0));
	frag_UV = aUV;
	
	gl_Position = vec4(frag_POS, 1.0);
}
```
Обьясняя просто - шейдер получает на вход позицию `aPos` и координаты текстур `aUV` в `layout`, а также их выводит через `out`. Дополнительно шейдеру выдаётся матрица `model`, но уже моделью, а не движком отрисовки.  
Функция `main()` работает также как и в C++, то есть исполняется при начале исполнения. Внутри неё выставляются значения на вывод, а также `gl_Position`, что отвечает за вывод позиции точки треугольника на экран.

## Фрагментный шейдер
В добавку к вертексному шейдеру создайте файл `shader.fs` с содержимым:
``` glsl
#version 330 core
out vec4 frag_COLOR;

in vec3 frag_POS;
in vec2 frag_UV;

struct Material {
	sampler2D diffuse0;
	bool diffuse0_isMonochrome;
	vec4 diffuse;
};
uniform Material material;
uniform bool noTextures;

void main() {
	// material values
	vec4 diffMap = material.diffuse;
	float transparency = 1.0;
	// texture values
	if(!noTextures) {
		diffMap = texture(material.diffuse0, frag_UV) * material.diffuse;
		if(material.diffuse0_isMonochrome)
			diffMap = vec4(vec3(diffMap.r),1);
		transparency = diffMap.a;
		if(transparency<0.1) discard;
	}
	frag_COLOR = vec4(diffMap.rgb, transparency);
}
```
Firesteel работает через материалы, то есть все значения текстур передаются в структуру `Material`. В шейдере должен быть свой адаптер для такого материала. В данном примере обрабатывается лишь карта рассеивания, однако Firesteel выдает значения и под карту нормалей, отражений, свечения и т.д. Также если на модели нет текстур, то значение `noTextures` выставляется в правду.  
В `main()` просто принимаются и обрабатываются значения текстур, а также отменяется отрисовка, если прозрачность меньше 0,1.

## Изменения в коде приложения
А теперь слегка изменим `main.cpp`, добавив туда новый "материал":
``` cpp
#include "engine/include/firesteel.hpp"
using namespace Firesteel;

std::shared_ptr<Shader> shader;
Entity entity;

class ModelLoaderApp : public Firesteel::App {
    virtual void onInitialize() override {
        shader=std::make_shared<Shader>("shader.vs", "shader.fs");
        entity.load("backpack.obj");
        entity.setMaterialsShader(shader);
    }
    virtual void onUpdate() override {
        entity.draw();
    }
    virtual void onShutdown() override {
        entity.remove();
    }
};

int main() {
    return ModelLoaderApp{}.start();
}
```

# Промежуточный результат \#1
После всех этих махинаций вы получите окно с следующим содержимым:
![Зад рюкзака](https://github.com/xanytka-devs/firesteel/blob/main/docs/assets/loading_models1.png?raw=true)
Результат более вменяемый, но всё ещё далёк от истины. Давайте добавим камеру, чтобы хотя бы была преспектива.

# Камера
Начнём, пожалуй с `main.cpp` в этот раз:
``` cpp
#include "engine/include/firesteel.hpp"
using namespace Firesteel;

Material material
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, -90));
Entity entity;

class ModelLoaderApp : public Firesteel::App {
    virtual void onInitialize() override {
        shader=std::make_shared<Shader>("shader.vs", "shader.fs");
        entity.load("backpack.obj");
        entity.setMaterialsShader(shader);
        camera.update();
    }
    virtual void onUpdate() override {
        //Get variables, needed for a draw call.
        glm::mat4 proj = camera.getProjection();
        glm::mat4 view = camera.getView();
        //Draw the model.
        shader->enable();
        shader->setMat4("projection", proj);
        shader->setMat4("view", view);
        entity.draw();
    }
    virtual void onShutdown() override {
        entity.remove();
    }
};

int main() {
    return ModelLoaderApp{}.start();
}
```

## Изменения в вертексном шейдере
Да, для корректного отбражения необходимо изменить вертексный шейдер, причём фрагментный нам менять не надо - он не отвечает за положение модели. Вот обновлённое содержимое `shader.fs`:
``` glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;

out vec3 frag_POS;
out vec2 frag_UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	frag_POS = vec3(model * vec4(aPos, 1.0));
	frag_UV = aUV;
	
	gl_Position = projection * view * vec4(frag_POS, 1.0);
}
```

# Промежуточный результат \#2
Вот что вы должны увидеть в окне после запуска:
![Широкий рюкзак](https://github.com/xanytka-devs/firesteel/blob/main/docs/assets/loading_models2.png?raw=true)
Очень даже неплохо. Надо лишь исправть то, что он черезчур широкий.

# "Сужение" рюкзака
В действительности рюкзак более широкий из-за того, что камера не имеет данных о размерах окна и поэтому искажает итоговую картинку. Чинится это добавлением буквально одной линии в `main.cpp` в `onUpdate()` до действий с шейдором:
``` cpp
virtual void onUpdate() override {
    //Get and set variables, needed for a draw call.
    glm::mat4 proj = camera.getProjection();
    glm::mat4 view = camera.getView();
    camera.aspect = window.aspect(); // <---
    //Draw the model.
    shader->enable();
    shader->setMat4("projection", proj);
    shader->setMat4("view", view);
    entity.draw();
}
```

# Промежуточный результат \#3
![И всё было хорошо у рюкзака](https://github.com/xanytka-devs/firesteel/blob/main/docs/assets/loading_models3.png?raw=true)
В принципе это всё, но почему бы не поэкспериментировать ещё чуть-чуть?

# Режимы отрисовки
В отрисовщике (рендерере) Firesteel поддерживается изменение режима отрисовки. Почему бы не попробывать их потыкать?  
Для этого внесём одну строку в `main.cpp`, а именно в `onInitialize()`:
``` cpp
virtual void onInitialize() override {
    shader=std::make_shared<Shader>("shader.vs", "shader.fs");
    entity.load("backpack.obj");
    entity.setMaterialsShader(shader);
    camera.update();
    renderer.setDrawMode(Renderer::DM_WIRE); // <---
}
```

# Итоговый результат \#4
![Треугольная сетка рюкзака](https://github.com/xanytka-devs/firesteel/blob/main/docs/assets/loading_models4.png?raw=true)
Это конечно великолепно, но почему бы не сделать изменение режима по нажатию кнопок?

# Заключение
Итоговый код вы можете найти [вот тут](https://github.com/xanytka-devs/fs-examples/blob/main/ModelLoading/main.cpp).