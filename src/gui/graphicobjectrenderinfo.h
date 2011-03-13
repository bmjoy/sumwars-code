#ifndef GRAPHIC_OBJECT_RENDER_INFO_H
#define GRAPHIC_OBJECT_RENDER_INFO_H
#include <OgreVector3.h>

#include "geometry.h"
#include <string>
#include <list>
#include <map>



struct ActionRenderpart
{
	enum Type
	{
		NONE=0,
		ANIMATION = 1,
		ROTATION = 2,
		MOVEMENT = 3,
		SCALE = 4,
		VISIBILITY = 5, 
		DETACH = 10,
  
		SOUND = 20,
	};


	std::string m_objectname;
	std::string m_animation;


	float m_start_time;
	float m_end_time;

	float m_start_val;
	float m_end_val;

	Ogre::Vector3 m_start_vec;
	Ogre::Vector3 m_end_vec;

	Type m_type;
};

struct MovableObjectInfo
{
	MovableObjectInfo()
	{
		m_type = SUBOBJECT;
		m_objectname="";
		m_source = "";
		m_bone = "";
		m_position = Ogre::Vector3(0,0,0);
		m_rotation = Ogre::Vector3(0,0,0);
		m_scale = 1.0f;
		m_highlightable = true;
	}

	enum Type
	{

		ENTITY = 1,
		PARTICLE_SYSTEM = 2,
 		BILLBOARD_SET = 3,
		BILLBOARD_CHAIN = 4,

		SUBOBJECT = 10,
  
		SOUNDOBJECT = 20,
	};

	Type m_type;
	std::string m_objectname;
	std::string m_source;
	std::string m_bone;
	Ogre::Vector3 m_position;
	Ogre::Vector3 m_rotation;
	float m_scale;
	
	bool m_highlightable;
};

/**
 * \struct ActionRenderInfo
 * \brief Struktur mit den Informationen zum Rendern einer einzelnen Aktion
 */
struct ActionRenderInfo
{
	ActionRenderInfo()
	: m_renderparts(), m_new_objects()
	{

	}

	/**
	 * \var std::list< ActionRenderpart > m_renderparts
	 * \brief einzelne Bestandteile des Rendervorgangs
	 */
	std::list< ActionRenderpart > m_renderparts;

	/**
	 * \var std::list< std::pair<float,MovableObjectInfo> > m_new_objects
	 * \brief Waehrend dieser Aktion neu angehaengte erzeugte Elemente
	 */
	std::list< std::pair<float,MovableObjectInfo> > m_new_objects;

	float m_time;
};


/**
 * \class GraphicRenderInfo
 * \brief Struktur mit den Informationen zum Rendern eines GraphicObject
 */
class GraphicRenderInfo
{
	public:
		enum InheritedParts
		{
			INHERIT_ALL = 0xffffffff,
			INHERIT_ANIMATION = 0x1,
			INHERIT_ROTATION = 0x2,
			INHERIT_MOVEMENT = 0x4,
			INHERIT_SCALE = 0x8,
			INHERIT_TRANSFORM = 0xe,
			INHERIT_SOUND = 0x80,
   
			INHERIT_OBJECTS = 0x100,
		};
		
		GraphicRenderInfo(std::string parent);

		~GraphicRenderInfo();

		void addObject(MovableObjectInfo& minfo)
		{
			m_objects.push_back(minfo);
		}

		void addActionRenderInfo(std::string action, ActionRenderInfo* info)
		{
			m_action_infos.insert(std::make_pair(action,info));
		}

		void addActionReference(std::string action, std::string refaction)
		{
			m_action_references[action] = refaction;
		}

		std::list<MovableObjectInfo>& getObjects()
		{
			return m_objects;
		}

		ActionRenderInfo* getActionRenderInfo(std::string action,int m_random_action_nr=1);

		ActionRenderInfo* getInheritedActionRenderInfo(std::string action,int m_random_action_nr=1);

		ActionRenderInfo* getOwnActionRenderInfo(std::string action,int m_random_action_nr=1);
		
		GraphicRenderInfo* getParentInfo();
		
		/**
		 * \brief Clears the pointer to the parent GraphicRenderInfo
		 * This forces the Renderinfo to update the parent on the next use, allowing to replace the parent RenderInfo
		 */
		void clearParentInfoPtr()
		{
			m_parent_ptr = 0;
		}
		
		void setInheritMask(unsigned int mask)
		{
			m_inherit_mask = mask;	
		}
		
		bool checkActionInheritMask(ActionRenderpart::Type arpart);
		
		bool checkInheritMask(unsigned int mask)
		{
			return m_inherit_mask & mask;
		}

	private:
		/**
		* \var std::list<MovableObjectInfo> m_objects
		* \brief Graphische Grundbestandteile des Objekts
		*/
		std::list<MovableObjectInfo> m_objects;

		/**
		* \var std::multimap<std::string, ActionRenderInfo*> m_action_infos
		* \brief Informationen, wie Aktionen zu rendern sind
		*/
		std::multimap<std::string, ActionRenderInfo*> m_action_infos;

		/**
		 * \var std::map<std::string, std::string> m_action_references
		 * \brief Referenzen von Aktionen fuer die keine expliziten Informationen vorliegen, auf andere Aktionen
		 */
		std::map<std::string, std::string> m_action_references;

		/**
		 * \var std::string m_parent
		 * \brief Renderinfo Objekt, von dem diese Objekt Aktionen erben kann
		 */
		std::string m_parent;

		/**
		 * \var GraphicRenderInfo* m_parent_ptr
		 * \brief Zeiger auf das VaterObjekt
		 */
		GraphicRenderInfo* m_parent_ptr;
		
		/**
		 * \var unsigned int m_inherit_mask
		 * \brief Maske, die bestimmt welche geerbten Renderanweisungen ausgefuehrt werden
		 * Bei Gegenstaenden wird haeufig verlangt, dass zwar die Animationen uebernommen werden, aber keine neuen Objekte (Partikel etc) angehaengt werden
		 **/
		unsigned int m_inherit_mask;
};

#endif
