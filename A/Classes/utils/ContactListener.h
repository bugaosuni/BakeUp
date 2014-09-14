#ifndef __CONTACT_LISTENER_H__
#define __CONTACT_LISTENER_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

struct Box2dContact
{
 b2Fixture *fixtureA;
 b2Fixture *fixtureB;
 
 bool operator==(const Box2dContact &other) const{
  return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
 }
};

class ContactListener : public b2ContactListener
{
public:
    ContactListener();
    ~ContactListener();

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);

    std::vector<Box2dContact> m_contacts;
};
#endif // __CONTACT_LISTENER_H__