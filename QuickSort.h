#pragma once

#include "Sphere.h"
#include "Const.h"

void Swap(unsigned int ind1, unsigned int ind2, float* Distances, Sphere* Spheres) {
	float tempDist = Distances[ind1];
	Sphere tempSphere = Spheres[ind1];

	Distances[ind1] = Distances[ind2];
	Spheres[ind1] = Spheres[ind2];

	Distances[ind2] = tempDist;
	Spheres[ind2] = tempSphere;
}

unsigned int Partition(unsigned int low, unsigned int high, float* Distances, Sphere* Spheres) {
	unsigned int pivotIndex = low;
	float pivotValue = Distances[high];

	for (unsigned int i = low; i < high; i++) {
		if (Distances[i] < pivotValue) {
			Swap(i, pivotIndex, Distances, Spheres);
			pivotIndex++;
		}
	}
	Swap(pivotIndex, high, Distances, Spheres);
	return pivotIndex;
}

void QuickSort(unsigned int low, unsigned int high ,float* Distances, Sphere* Spheres) {
	if (low < high) {
		unsigned int part = Partition(low, high, Distances, Spheres);
		if (part>0) QuickSort(low, part - 1, Distances, Spheres);
		QuickSort(part + 1, high, Distances, Spheres);
	}
}