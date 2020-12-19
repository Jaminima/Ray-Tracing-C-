#pragma once

#include "Sphere.h"
#include "Const.h"

void Swap(unsigned int ind1, unsigned int ind2, float* Distances, SceneObject* SceneObjects) {
	float tempDist = Distances[ind1];
	SceneObject tempObject = SceneObjects[ind1];

	Distances[ind1] = Distances[ind2];
	SceneObjects[ind1] = SceneObjects[ind2];

	Distances[ind2] = tempDist;
	SceneObjects[ind2] = tempObject;
}

unsigned int Partition(unsigned int low, unsigned int high, float* Distances, SceneObject* SceneObjects) {
	unsigned int pivotIndex = low;
	float pivotValue = Distances[high];

	for (unsigned int i = low; i < high; i++) {
		if (Distances[i] < pivotValue) {
			Swap(i, pivotIndex, Distances, SceneObjects);
			pivotIndex++;
		}
	}
	Swap(pivotIndex, high, Distances, SceneObjects);
	return pivotIndex;
}

void QuickSort(unsigned int low, unsigned int high, float* Distances, SceneObject* SceneObjects) {
	if (low < high) {
		unsigned int part = Partition(low, high, Distances, SceneObjects);
		if (part > 0) QuickSort(low, part - 1, Distances, SceneObjects);
		QuickSort(part + 1, high, Distances, SceneObjects);
	}
}
