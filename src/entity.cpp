#include "entity.hpp"

void initBody(b2Body* body, Kind entityKind, void* object)
{
    UserdataTag* tag = new UserdataTag{entityKind, object};

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(tag);
}
