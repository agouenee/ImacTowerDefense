#ifndef ITD_TOUR___
#define LDR_TOUR___

typedef enum {
	ROCKET, LASER, MITRAILLETTE, HYBRIDE
}TourType;

typedef struct {
	TourType type;
	unsigned int puissance;
	unsigned int portee;
	unsigned int cadence;
	unsigned int prix;
}Tour;

#endif