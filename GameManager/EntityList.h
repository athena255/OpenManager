#pragma once
// EntityList
class GAMEMANAGER_API CEntityList {
public:
	// Return the first entity
	virtual CEntity* first_entity() const = 0;
	// Number of entities
	virtual size_t num_entities() const = 0;
	// Get entity at the specified index
	virtual CEntity* get_entity(size_t i) = 0;
	// Return a pointer to the next entity
	virtual CEntity* next_entity(CEntity* in_current_entity) const = 0;
};