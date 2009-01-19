#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <map>
#include <sstream>

extern "C"
{
	
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

/**
 * Typ eines Triggers
 */
typedef std::string TriggerType;

/**
 * \class Trigger
 *  \brief Ausloeser fuer ein Event
 */
class Trigger
{
	public:
		/**
		 * \fn Trigger(TriggerType type)
		 * \brief Konstruktor
		 * \param type Typ des Ausloesers
		 */
		Trigger(TriggerType type);
		
		/**
		 * \fn void addVariable(std::string name, int value)
		 * \brief Fuegt eine Integer Variable hinzu
		 * \param name Name der Variable
		 * \param value Wert der Variable
		 */
		void addVariable(std::string name, int value);
		
		/**
		 * \fn void addVariable(std::string name, bool value)
		 * \brief Fuegt eine Integer Variable hinzu
		 * \param name Name der Variable
		 * \param value Wert der Variable
		 */
		void addVariable(std::string name, bool value);
		
		/**
		 * \fn void addVariable(std::string name, float value)
		 * \brief Fuegt eine float Variable hinzu
		 * \param name Name der Variable
		 * \param value Wert der Variable
			*/
		void addVariable(std::string name, float value);
		
			/**
		 * \fn void addVariable(std::string name, std::string value)
		 * \brief Fuegt eine String Variable hinzu
		 * \param name Name der Variable
		 * \param value Wert der Variable
			 */
		void addVariable(std::string name, std::string value);
		
		/**
		 * \fn std::string& getLuaVariables()
		 * \brief Gibt Referenz auf den String mit den Variablen fuer Lua aus
		 */
		std::string& getLuaVariables()
		{
			return m_lua_variables;
		}
		
		/**
		 * \fn TriggerType getType()
		 * \brief Gibt den Typ des Triggers aus
		 */
		TriggerType getType()
		{
			return m_type;
		}
		
	private:
		/**
		 * \var TriggerType m_type
		 * \brief Typ des Ausloesers
		 */
		TriggerType m_type;
		
		/**
		 * \var std::string m_lua_variables
		 * \brief String der in Lua die Variablen setzt, die dem Trigger mitgegeben wurden
		 */
		std::string m_lua_variables;
};


/**
 * \class Event
 * \brief Klasse fuer ein Ereignis
 */
class Event
{
	public:
		/**
		 * \fn Event()
		 * \brief Konstruktor
		 */
		Event()
		{
			m_once = false;
			m_condition = LUA_NOREF;
			m_effect = LUA_NOREF;
		}
		
		/**
		 * \fn ~Event()
		 * \brief Destruktor
		 */
		~Event();
		
		/**
		 * \fn void setCondition(const char * cond)
		 * \brief Setzt die Bedingung fuer die Ausfuehrung des Events
		 * \param cond Bedingung als Folge von Lua Anweisungen
		 */
		void setCondition(const char * cond);
	
		/**
		 * \fn bool checkCondition();
		 * \brief Testet die Bedingung fuer die Ausfuehrung des Events
		 */
		bool checkCondition();
		
		/**
		 * \fn void setEffect(const char * effect)
		 * \brief Setzt die Wirkung des Events
		 * \param effect Wirkung als Folge von Lua Anweisungen
		 */
		void setEffect(const char * effect);
		
		/**
		 * \fn void doEffect();
		 * \brief Fuehrt die Wirkung des Events aus
		 */
		void doEffect();
		
		/**
		 * \fn void setOnce(bool once = true)
		 * \brief Setzt, ob das Event nach der ersten Ausfuehrung geloescht wird
		 * \param once wenn auf true gesetzt, wird das Event nach der ersten Ausfuehrung geloescht
		 */
		void setOnce(bool once = true)
		{
			m_once = once;
		}
		
		/**
		 * \fn bool getOnce()
		 * \brief Gibt aus, das Event nach der ersten Ausfuehrung geloescht wird
		 */
		bool getOnce()
		{
			return m_once;
		}
		
	private:
		/**
	 	 * \var int m_condition
		 * \brief Bedingung unter der das Event ausgefuehrt wird (Folge von Lua Anweisungen)
		 */
		int m_condition;
		
		/**
		 * \var bool m_once
		 * \brief wenn auf true, gesetzt, wird das Event nach der ersten Ausfuehrung geloescht
		 */
		bool m_once;
		
		/**
		 * \var int m_effect
		 * \brief Wirkung des Events (Folge von Lua Anweisungen)
		 */
		int m_effect;
};


#endif

