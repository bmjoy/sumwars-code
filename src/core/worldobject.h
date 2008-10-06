/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <string>
#include <cstring>
#include <sstream>
#include "debug.h"
#include "charconv.h"
#include "event.h"
#include <map>
#include <list>

class World;
class Region;
struct Damage;


/**
 * \file worldobject.h
 * \defgroup worldobject \Weltobjekt
 * \ingroup world
 * \brief Deklaration des WorldObject
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Bietet der Welt das WorldObject an
 */

/**
 * \struct GridLocation
 * \brief Informationen zur Position eines Objektes in der Grid-Datenstruktur der Welt
 */
struct GridLocation
{
	
	/**
	 * \var m_region
	 * \brief Region in der sich das Objekt befindet
	 */
	short m_region;
	
	/**
	 * \var m_grid_x
	 * \brief Index des Objektes im Grid in x-Richtung
	 */
	int m_grid_x;
	
	/**
	 * \var m_grid_y
	 * \brief Index des Objektes im Grid in y-Richtung
	 */
	int m_grid_y;
};



/**
 * \struct Shape
 * \brief Repraesentiert eine Geometrische Form, moeglich sind Rechteck und Kreis
 */

struct Shape
{
	/**
	 * \enum ShapeType
	 * \brief Typ der geometrischen Form
	 */
	enum ShapeType
	{
		RECT=1,
		CIRCLE=2
	};
	/**
	 * \var m_coordinate_x
	 * \brief x-Koordinate des Objekts (Mittelpunkt)
	 */
	float m_coordinate_x;

	/**
	 * \var m_coordinate_y
	 * \brief x-Koordinate des Objekts (Mittelpunkt)
	 */
	float m_coordinate_y;


	/**
	 * \var m_type
	 * \brief Form des Objektes (kreis / rechteckfoermig)
	 */
	ShapeType m_type;

	/**
	 * \var m_extent_x
	 * \brief Ausdehnung in x-Richtung (nur bei recheckigen Objekten verwendet)
	 */
	float m_extent_x;

	/**
	 * \var m_extent_y
	 * \brief Ausdehnung in y-Richtung (nur bei recheckigen Objekten verwendet)
	 */
	float m_extent_y;

	/**
	 * \var m_radius
	 * \brief Radius des Objektes ( nur bei kreisfoermigen Objekten verwendet)
	 */
	float m_radius;
};

/**
 * \class WorldObject
 * \brief ein Objekt, welches sich in der Spielwelt befindet
 */
class WorldObject {

	public:

	/**
	 * \enum Relation
	 * \brief Listet die verschiedenen Beziehungen zwischen Parties auf
	 */
	enum Relation
	{
		NEUTRAL = 0,
		ALLIED = 1,
 		HOSTILE =2
	};

	
	/**
	 * \enum Group
	 * \brief Listet verschiedene Grundtypen von Objekten auf
	 */
	enum Group
	{
		CREATURE_ONLY = 1,
		PLAYER=2,
		CREATURE = 3,
  		FIXED = 4,
  		DEAD = 8,
  		GROUP_ALL = 15
	};
	
	/**
	* \struct Geometry
	* \brief Informationen zur Geometrie eines Objektes
	*/
	struct Geometry
	{
		/**
		* \enum Layer
		* \brief TODO
		*/
		enum Layer
		{
			LAYER_BASE =0x01,
			LAYER_AIR = 0x02,
			LAYER_DEAD = 0x04,
   /*
			LAYER_FIXED =  0x1F,
			LAYER_CREATURE = 0x6F,
			LAYER_MONSTER=  0x2F,
			LAYER_PLAYER = 0x4F,
   */
			LAYER_ALL = 0x0F,
   
	//		LAYER_ALL_TYPES = 0xF0,
			LAYER_SPECIAL = 0x10,
		};


		//Fields
		/**
		* \var m_shape
		* \brief Form des Objektes
		*/
		Shape m_shape;

		/**
		* \var m_angle
		* \brief Winkel um den das Objekt gegenueber dem Koordinatensystem gedreht ist
		*/
		float m_angle;

		/**
		* \var m_layer
		* \brief Ebene in der sich das Objekt befindet
		*/
		short m_layer;
	};

	/**
	 * \struct TypeInfo
	 * \brief Enthaelt alle Informationen zum Typ eines Objektes
	 */
	struct TypeInfo
	{
		/**
		 * \enum ObjectType
		 * \brief Grundtypen von Objekten
		 */
		enum ObjectType
		{
			TYPE_NONE=0,
			TYPE_PLAYER =1,
			TYPE_MONSTER= 2,
			TYPE_TRADER=3,
			TYPE_FIXED_OBJECT=4
		};

		/**
		 * \var ObjectSubtype
		 * \brief genauere Unterteilung von Objekten
		 */
		typedef std::string ObjectSubtype;

		/**
		 * \enum Category
		 * \brief Verschiedene Kategorien von Lebewesen
		 */
		enum Category
		{
			NOCATEGORY =0,
			HUMAN = 0x10,
			DEMON = 0x20,
			UNDEAD = 0x40,
			DWARF = 0x80,
			DRAKE = 0x100,
			FAIRY = 0x200,
			GOBLIN = 0x400,
			ANIMAL = 0x800,
			SUMMONED = 0x1000
		};

		/**
		 * \enum Fraction
		 * \brief Parteien fuer die Lebewesen kaempfen koennen
		 */
		enum Fraction
		{
			NOFRACTION = 0,
			FRAC_HUMAN = 1,
			FRAC_DEMON = 2,
			FRAC_UNDEAD = 3,
			FRAC_DWARF = 4,
			FRAC_SUMMONER = 5,
			FRAC_MONSTER = 6,
			FRAC_HOSTILE_TO_ALL=9,
			FRAC_PLAYER_PARTY = 10,
		};



		/**
		* \var m_type
		* \brief Informationen zum Objekttyp, unter anderem welcher abgeleiteten Klasse das Objekt angeh&ouml;rt
		*/
		ObjectType m_type;

		/**
		* \var m_subtype;
		* \brief Gibt den Untertyp des Objektes an
		*/
		ObjectSubtype m_subtype;

		/**
		 * \var Category m_category
		 * \brief Kategorie des Objektes
		 */
		Category m_category;

		/**
		 * \var Fraction m_fraction
		 * \brief Fraktion des Objektes
		 */
		Fraction m_fraction;

	};

	/**
	 * \struct MoveInfo
	 * \brief Informationen zum aktuelle Bewegungszustand eines Objektes
	 */
	struct MoveInfo
	{
			/**
		* \var m_speed_x
		* \brief TODO
			*/
		float m_speed_x;

		/**
		* \var m_speed_y
		* \brief TODO
		*/
		float m_speed_y;
	};

	/**
	 * \enum State
	 * \brief zaehlt Zustaende von Objekten auf
	 */
	enum State
	{
			STATE_INACTIVE=0,
			STATE_ACTIVE =1,
			STATE_DIEING =2,
			STATE_DEAD =3,
			STATE_REGION_DATA_REQUEST=6,
			STATE_REGION_DATA_WAITING=4,
			STATE_ENTER_REGION =5,
			STATE_QUIT=9,
			STATE_STATIC = 10,

	};

	/**
	 * \enum SpecialFlags
	 * \brief zaehlt Spezialzustaende und -eigenschaften von Objekten auf
	 */
	enum SpecialFlags
	{
		FIRESWORD = 0x1,
		FLAMESWORD = 0x2,
		FLAMEARMOR = 0x4,
		DECOY = 0x8,
		SCARE = 0x10,
		CRIT_HITS = 0x20,
		ICE_ARROWS = 0x40,
		FROST_ARROWS = 0x80,
		WIND_ARROWS = 0x100,
		STORM_ARROWS = 0x200,
		WIND_WALK = 0x400,
		BURNING_RAGE = 0x800,
		STATIC_SHIELD = 0x1000

	};




	//Fields
	//Constructors
	/**
	 * \fn WorldObject(World* world,int id)
	 * \brief Konstruktor
	 * \param world World Objekt, in dem sich das neue WorldObject befindet
	 * \param id ID des WorlObject
	 *
	 * Erstellt ein neues WorldObject
	 */
	WorldObject(World* world, int id);

	/**
	 * \fn virtual ~WorldObject()
	 * \brief Destruktor
	 */
	virtual ~WorldObject()
	{
	}
	
	/**
	 * \fn getWorld()
	 * \brief Gibt einen Zeiger auf die Welt zurueck
	 * \return Zeiger auf die Welt
	 */
	World* getWorld()
	{
		return m_world;
	}
	
	/**
	 * \fn Region* getRegion()
	 * \brief Gibt die Region aus, in der sich das Objekt befindet
	 */
	Region* getRegion();
	
	/**
	 * \fn GridLocation* getGridLocation()
	 * \brief Gibt den Ort des Objektes im Grid aus
	 * \return Ort des Objektes im Grid
	 */
	GridLocation* getGridLocation()
	{
		return &m_grid_location;
	}
	
	/**
	 * \fn MoveInfo* getMoveInfo()
	 * \brief Gibt Zeiger auf aktuellen Bewegungszustand zurueck
	 * \return aktueller Bewegungszustand
	 */
	MoveInfo* getMoveInfo()
	{
		return &m_move_info;
	}
	
	//Operations
	/**
	 * \fn virtual bool init ()
	 * \brief Initialisiert das WorldObject. Die Funktion ist virtuell und wird durch die abgeleiteten Klassen &uuml;berschrieben.
	 * \return bool, der angibt ob die initialisierung erfolgreich war
	 */
	virtual  bool  init ()
	{
		m_destroyed=false;
		return true;
	}
	
	
	/**
	 * \fn virtual bool update ( float time)
	 * \brief Aktualisiert das WorldObject, nachdem eine bestimmte Zeit vergangen ist. Alle Aktionen des Objekts werden auf diesem Weg ausgel&ouml;st. Die Funktion ist virtuell und wird von den abgeleiteten Klassen &uuml;berschrieben
	 * \param time Menge der vergangenen Zeit in Millisekunden
	 * \return bool, der angibt, ob die Aktualisierung fehlerfrei verlaufen ist
	 */
	virtual  bool  update ( float time) 
	{
		return true;
	}
	
	
	/**
	 * \fn virtual bool destroy()
	 * \brief Zerst&ouml;rt das WorldObject.  Die Funktion ist virtuell und wird von den abgeleiteten Klassen &uuml;berschrieben
	 * \return bool der angibt, ob die Zerst&ouml;rung erfolgreich war
	 */
	virtual  bool  destroy ();
	
	/**
	 * \fn bool getDestroyed()
	 * \brief Gibt zurueck ob das Objekt zerstoert ist
	 */
	bool getDestroyed()
	{
		return m_destroyed;
	}
	
	/**
	 * \fn void setDestroyed(bool d)
	 * \brief Setzt den Status zerstoert
	 * \param d neuer Status
	 */
	void setDestroyed(bool d)
	{
		m_destroyed =d;
	}
	
	/**
	 * \fn void moveTo(float x, float y)
	 * \brief setzt die x- und die y-Koordinate
	 * \param x x-Koordinate
	 * \param y y-Koordinate
	 * 
	 * Diese Funktion verschiebt das Objekt an einen anderen Ort. Wenn das Objekt in einem Quadtree eingeordnet ist, so muss bei jeder &Auml;nderung der Koordinaten getestet werden, ob das Objekt im Quadtree neu angeordnet werden muss. Diese Funktion teilt dem Quadtree die &Auml;nderung der Koordinanten mit und l&ouml;st damit ebentuell eine Reorganisation aus. Daher sollte ausserhalb des Quadtree diese Funktion zum ver&auml;ndern der Koordinaten verwendet werden.
	 */
	bool moveTo(float x, float y);
	
	/**
	 * \fn void takeDamage(Damage* damage)
	 * \brief Das Objekt nimmt Schaden in der in damage angegebenen Art und Hoehe.
	 * \param damage Schadensart und -hoehe
	 */
	virtual void takeDamage(Damage* damage)
	{
	}
	
	/**
	 * \fn virtual void writeEvent(Event* event, CharConv* cv)
	 * \brief Schreibt die Daten zu einem Event in den Bitstream
	 * \param event Event das beschrieben wird
	 * \param cv Bitstream
	 */
	virtual void writeEvent(Event* event, CharConv* cv)
	{
	}
	
	/**
	 * \fn virtual void processEvent(Event* event, CharConv* cv)
	 * \brief Fuehrt die Wirkung eines Events auf das Objekt aus. Weitere Daten werden aus dem Bitstream gelesen
	 */
	virtual void processEvent(Event* event, CharConv* cv)
	{
	}
	
	/**
	 * \fn virtual bool reactOnUse (int id)
	 * \brief Diese Funktion behandelt die Reaktion eines Worldobject, wenn auf es eine Aktion ausgef&uuml;hrt wird.
	 * \param id Id des Spielers, der die Aktion ausf&uuml;hrt
	 * \return bool, der angibt, ob die Behandlung der Aktion erfolgreich war
	 */
	virtual  bool  reactOnUse (int id)
	{
		return true;
	}
	
	/**
	 * \fn int getEventMask()
	 * \brief Gibt die Bitmaske der Events aus
	 */
	int getEventMask()
	{
		return m_event_mask;
	}
	
	/**
	 * \fn void clearEventMask()
	 * \brief Setzt die Bitmaske der Events auf 0
	 */
	void clearEventMask()
	{
		m_event_mask =0;
	}

	//Accessor Methods
    /**
	 * \fn void setSubType(TypeInfo::ObjectSubtype t)
	 * \brief Setzt den Subtyp des Objektes
	 * \param t neuer Subtyp
	 */
    void setSubType(TypeInfo::ObjectSubtype t)
    {
        m_type_info.m_subtype =t;
    }


	/**
	 * \fn Geometry* getGeometry()
	 * \brief Gibt einen Zeiger auf die Struktur mit den Basisinformationen zurueck
	 * \return Zeiger auf die Struktur
	 */
	Geometry* getGeometry()
	{
		return &m_geometry;
	}

	/**
	 * \fn TypeInfo* getTypeInfo()
	 * \brief Gibt einen Zeiger auf die Struktur mit den Typinformationen zurueck
	 * \return Zeiger auf die Struktur
	 */
	TypeInfo* getTypeInfo()
	{
		return &m_type_info;
	}

	/**
	 * \fn State getState()
	 * \brief Gibt den aktuellen Status des Objektes aus
	 * \return Status des Objektes
	 */
	State getState()
	{
		return m_state;
	}

	/**
	 * \fn void setState(State s)
	 * \brief Setzt den Status
	 * \param s Status
	 */
	void setState(State s)
	{
		m_state = s;
	}

	/**
	 * \fn int getId()
	 * \brief Gibt die ID des objektes zurueck
	 * \return ID
	 */
	int getId()
	{
		return m_id;
	}
	
	/**
	 * \fn void setId(int id)
	 * \brief Setzt die ID eines Objektes
	 */
	void setId(int id)
	{
		m_id = id;
	}

	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	 */
	virtual void toString(CharConv* cv);


	/**
	 * \fn virtual void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	virtual void fromString(CharConv* cv);

	/**
	 * \fn std::string getName()
	 * \brief Gibt den Name des Objektes aus
	 */
	virtual std::string getName();


	/**
	 * \fn std::string getNameId()
	 * \brief Gibt Name mit angehaengter ID aus
	 */
	std::string getNameId();

	protected:
		
	/**
	 * \var int m_event_mask
	 * \brief Bitmaske mit den Events die das Objekt seit dem letzten Update erlebt hat
	 */
	int m_event_mask;
		
//Private stuff
private:

	/**
	 * \var Geometry m_geometry
	 * \brief Beinhaltet alle  Daten zur Geometrie des  Objekts
	 */
	Geometry m_geometry;

	/**
	 * \var TypeInfo m_type_info
	 * \brief Beeinhaltet alle Informationen zum Typ des Objektes
	 */
	TypeInfo m_type_info;

	/**
	 * \var m_state;
	 * \brief Gibt den Status des Objekts an
	 */
	State m_state;

	/**
	 * \var m_id
	 * \brief ID des Objekts
	 */
	int m_id;

	/**
	 * \var GridLocation m_grid_location
	 * \brief Ort des Objektes in der internen Struktur
	 */
	
	GridLocation m_grid_location;
	 /**
	 * \var m_move_info
	 * \brief Information ueber den Bewegungszustand
	  **/
	MoveInfo m_move_info;
	
	/**
	 * \var m_destroyed
	 * \brief Information, ob das Objekt zerst&ouml;rt ist
	 */
	bool m_destroyed;
	
	/**
	 * \var m_world
	 * \brief Zeiger auf das World Objekt, in dem das aktuelle Objekt eingeordnet ist
	 */
	World* m_world;

};

#include "damage.h"

/**
 * Liste von WorldObject Zeigern sortiert nach ihrer ID
 */
typedef std::map<int,WorldObject*> WorldObjectMap;
	
/**
 * Liste von WorldObject Zeigern
 */
typedef std::list<WorldObject*> WorldObjectList;
				 
#endif //WORLDOBJECT_H
