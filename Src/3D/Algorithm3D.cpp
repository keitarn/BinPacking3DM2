#include "3D/Debug.h"
#include "3D/Algorithm3D.h"

#define ECART_CONTENEURS 80
#define TYPE_DECOUPE 2

static bool comp(const Composant3D* a, const Composant3D* b) {
	return (b->getCoteX() * b->getCoteY() * b->getCoteZ() < a->getCoteX() * a->getCoteY() * a->getCoteZ());
}

Algorithm3D::Algorithm3D(std::list<Composant3D*> composantsMain, std::list<Conteneur3D*> conteneursMain,
	std::list<Conteneur3D*> conteneursDispoMain) : composants3D(composantsMain), conteneurs3D(conteneursMain),
	conteneursDispo3D(conteneursDispoMain) {
	composants3D.sort(comp);
}

Algorithm3D::~Algorithm3D() {
	std::list<Conteneur3D*>::iterator it;
	for (it = conteneurs3D.begin(); it != conteneurs3D.end(); it++) {
		delete* it;
	}
	conteneurs3D.clear();
	std::list<Composant3D*>::iterator it2;
	for (it2 = composants3D.begin(); it2 != composants3D.end(); it2++) {
		delete* it2;
	}
	composants3D.clear();

	std::list<Conteneur3D*>::iterator it3;
	for (it3 = conteneursDispo3D.begin(); it3 != conteneursDispo3D.end(); it3++) {
		delete* it3;
	}
	conteneursDispo3D.clear();
}

std::list<Composant3D*> Algorithm3D::calculRangement() {
	float longueurCourante = 0;
	std::list<Composant3D*>::iterator comp;
	std::list<Composant3D*>::iterator compErase;
	Position3D* posCourante = DBG_NEW Position3D(0, 0, 0);
	std::list<Conteneur3D*>::iterator cont = conteneurs3D.begin();
	bool erase = false;
	while (cont != conteneurs3D.end() && !(composants3D.empty())) {
		posCourante->setX(longueurCourante);
		(*cont)->setPosition(posCourante->getX(), posCourante->getY(), posCourante->getZ());
		comp = composants3D.begin();
		for (comp; comp != composants3D.end(); comp++) {
			ArbreBinaire3D* arbre = (*cont)->rechercheLibre((*comp)->getCoteX(), (*comp)->getCoteY(), (*comp)->getCoteZ());
			if (arbre != nullptr) {
				if (arbre->getEspaceLibre()->getCoteX() < (*comp)->getCoteX() || arbre->getEspaceLibre()->getCoteZ() < (*comp)->getCoteZ()) {
					float xTemps = (*comp)->getCoteX();
					float zTemps = (*comp)->getCoteZ();
					(*comp)->setCoteX(zTemps);
					(*comp)->setCoteZ(xTemps);
				}
				//arbre->affichageArbre();
				if (erase) {
					composants3D.erase(compErase);
					erase = false;
				}

				(*comp)->setConteneur((*cont)->getId());
				(*comp)->setPosition(arbre->getEspaceLibre()->getPosition());
				(*cont)->addComposant(*comp);
				arbre->creationFils((*comp)->getCoteX(), (*comp)->getCoteY(), (*comp)->getCoteZ(), TYPE_DECOUPE);
				compErase = comp;
				arbre = nullptr;
				erase = true;
			}
		}
		if (erase) {
			composants3D.erase(compErase);
			erase = false;
		}
		longueurCourante = longueurCourante + ECART_CONTENEURS + (*cont)->getCoteX();
		cont++;

	}

	if (posCourante != nullptr) {
		delete posCourante;
		posCourante = nullptr;
	}
	return composants3D;
}

void Algorithm3D::setListeComposant(std::list<Composant3D*> composantsMain) {
	composants3D = composantsMain;
}

void Algorithm3D::setListeConteneur(std::list<Conteneur3D*> conteneursMain) {
	conteneurs3D = conteneursMain;
}
