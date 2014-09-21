#include "ContactListener.h"
#include "cocos2d.h"

USING_NS_CC;

ContactListener::ContactListener()
{

}

ContactListener::~ContactListener()
{

}

void ContactListener::BeginContact(b2Contact* contact)
{
    Box2dContact box2dContact = {contact->GetFixtureA(), contact->GetFixtureB()};

    m_contacts.push_back(box2dContact);
}

void ContactListener::EndContact(b2Contact* contact)
{
    Box2dContact box2dContact = {contact->GetFixtureA(), contact->GetFixtureB()};
    std::vector<Box2dContact>::iterator it = std::find(m_contacts.begin(), m_contacts.end(), box2dContact);
    
    if(it != m_contacts.end())
    {
        m_contacts.erase(it);
    }
}