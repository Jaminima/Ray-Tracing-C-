#pragma once
#include "SceneObject.h"

class SceneObjItem {
public:
	SceneObjItem* Next = 0x0;
	SceneObject* Obj = 0x0;

	SceneObjItem(SceneObject* Obj) {
		this->Obj = Obj;
	}
};

class SceneObjList {
public:
	SceneObjItem* Head = 0x0;

	void Add(SceneObject* Obj) {
		SceneObjItem* Temp = new SceneObjItem(Obj);
		Temp->Next = Head;
		Head = Temp;
	}
};