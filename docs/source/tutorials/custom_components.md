# Свои компоненты
Перед тем, как начать проверьте, не включены ли у вас в коде следующие флаги:  
- `FS_NO_COMPONENTS`  
- `FS_NO_REG_TYPES`

Если они включены, то лучше уберите их.

# Минимальный компонент
``` cpp
#include <firesteel/component.hpp>
using namespace Firesteel;

class MyComponent : public Component {
public:
	MyComponent(Entity* tEntity)
		: Component(tEntity) {
	}
	//Runs when component is added.
	void onStart() override {}
	//Runs every frame.
	void onUpdate() override {}
	//Runs before rendering.
	void onDraw() override {}
	//Runs when component gets removed.
	void onRemove() override {}

	void properties() override { }
	const char* name() const override { return "fs.my_component"; }
};
```
Тут добавлены все возможные методы для компонента. А также тут добавлен конструктор и два метода в конце - они должны быть обязательно.
Иначе данный компонент будет невозможно зарегистрировать в специальный реестр (ну или скорее сериализовать, но про это позже).  

## Регистрация компонентов
``` cpp
#include <firesteel/firesteel.hpp>
using namespace Firesteel;

#include "my_component.hpp"

class CustomComponentsApp : public Firesteel::App {
	void onInitialize() override {
		ComponentRegistry::sInstance()->append("fs.my_component", DefaultComponentFactory<MyComponent>);
	}
};
```
А вот тут по подробнее:
- `ComponentRegistry` - как раз таки реестр компонентов
- `ComponentRegistry::sInstance()->append()` - метод регистрации через статичный реестр. Т.е. вам не нужно создавать свою копию реестра для этого -
это уже сделано за вас
- `DefaultComponentFactory<MyComponent>` - базовая фабрика для компонента, если ваш компонент не требует дополнительных действий при добавлении на
сущность

## Альтернативная регистрация
Вообще второй аргумент в `ComponentRegistry::sInstance()->append()` это лямбда-функция. То есть вы вполне можете просто:
``` cpp
ComponentRegistry::sInstance()->append("fs.my_component", [](Entity* e, const nlohmann::json& j) {
	return Component::create<MyComponent>(e,j);
});
```

## Сериализация и десериализация
Если вы используете систему сцен, что уже есть в Firesteel, то всё уже готово. Регистрация в реестре уже даёт возможность
компоненту сериализовываться (т.е. сохраняться и загружаться). Если ваш компонент имеет имя другого, уже существующего
компонента, то вылетит ошибка.  
Хотите добавить сохраняемые свойства для компонента? Да проще простого. Добавьте в свой компонент следующее:
``` cpp
void properties() override {
	PROPERTY(value);
}

std::string value;
```
Также лучше поменять конструктор для вашего же удобства (да, это необязательно, но тогда вам в исполнении придётся самим назначать
значения для свойств, если добавляете компонент):
``` cpp
MyComponent(Entity* tEntity, const std::string& tVal="Hello World!")
	: value(tVal), Component(tEntity) { }
```
Ну и да, заместо типа `std::string` ещё могут быть и другие типы данных.  
Вот их список на данный момент:  
- bool  
- uint (т.е. unsigned int)  
- int  
- float  
- double  
- std::string  
- glm::vec2  
- glm::vec3  
- glm::vec4

# Свой сериализатор для типа данных
Если вам недостаточно изначальных типов сериализуемых данных, то можете сделать свой сериализатор:
``` cpp
#include <firesteel/type.hpp>
using namespace Firesteel;

struct MyType {
	bool boolVal;
};

struct MyTypeTypeHandler : public ITypeHandler {
	std::string toString(const void* tVal) const override {
		return (static_cast<const MyType*>(tVal)->boolVal)?"true":"false";
	}
	void fromString(void* tVal, const std::string& tStr) const override {
		static_cast<MyType*>(tVal)->boolVal=(tStr=="true");
	}
#ifndef FS_NO_JSON
	nlohmann::json toJson(void* tVal) const override {
		return static_cast<const MyType*>(tVal)->boolVal;
	}
	void fromJson(void* tVal, const nlohmann::json& tJson) const override {
		static_cast<MyType*>(tVal)->boolVal=tJson.get<bool>();
	}
#endif // !FS_NO_JSON
};
```
А также не забудьте его зарегистривовать:
``` cpp
class WindowApp : public Firesteel::App {
	void onInitialize() override {
		TypeRegistry::sInstance()->registerType<MyType>(std::make_unique<MyTypeTypeHandler>());
	}
}
```
И после этого вы можете использовать этот тип в компонентах как и остальные:
``` cpp
class MyComponent : public Component {
public:
	MyComponent(Entity* tEntity, const MyType& tVal=MyType{})
		: value(tVal), Component(tEntity) { }

	void properties() override {
		PROPERTY(value);
	}
	const char* name() const override { return "fs.my_component"; }

	MyType value;
};
```
Посмотрите на [уже существующие сериализаторы](https://github.com/xanytka-devs/firesteel/blob/main/include/firesteel/builtin_types.hpp),
если не понимаете как реализовать то или иное.