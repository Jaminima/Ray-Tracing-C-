#pragma once

class Item {
public:
	Item* Next = 0x0;
	void* Obj = 0x0;

	Item(void* Obj) {
		this->Obj = Obj;
	}
};

class List {
public:
	Item* Head = 0x0;

	void Add(void* Obj) {
		Item* Temp = new Item(Obj);
		Temp->Next = Head;
		Head = Temp;
	}
};