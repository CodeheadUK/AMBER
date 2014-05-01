#ifndef __HIT_LOCATION_H__
#define __HIT_LOCATION_H__

#include "equipment_location.h"

class hit_location
{
	private:

	char _name[16];
	int _structure;
	int _front_armour;
	int _rear_armour;
	bool has_rear_armour;
	equipment main_equipment[6];
	equipment sec_equipment[6];
	bool has_sec_equipment;

	hit_location child_location;
	hit_location parent_location;

	public:
	int damage(int value);


};

#endif
