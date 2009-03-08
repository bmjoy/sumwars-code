#ifndef ITEMWINDOW_H
#define ITEMWINDOW_H

#include "window.h"

/**
 * \class ItemWindow
 * \brief Fenster das Gegenstaende darstellen muss
 */
class ItemWindow : public Window
{
	public:
		/**
		 * \fn ItemWindow (Document* doc)
		 * \brief Konstruktor
		 */
		ItemWindow (Document* doc);
	
		/**
		 * \fn virtual void update()=0
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update() =0;

		
		/**
		 * \fn static void registerItemImage(Item::Subtype type, std::string image)
		 * \brief registiert ein Bild fuer ein bestimmten Itemtyp
		 * \param type Typ des Gegenstandes
		 * \param image Bild der Form set: ... image: ...
		 */
		static void registerItemImage(Item::Subtype type, std::string image)
		{
			m_item_images[type] = image;
		}
	
		/**
		 * \fn static std::string getItemImage(Item::Subtype type)
		 * \brief Gibt zu dem angegebenen Itemtyp das zugehoerige Bild aus
		 */
		static std::string getItemImage(Item::Subtype type);
		
		
	protected:
		
		
		/**
		 * \fn void updateItemWindowTooltip(CEGUI::Window* img, Item* item, Player* player, int gold=-1)
		 * \brief aktualisiert den Tooltip eines Items
		 * \param img CEGUI Fenster
		 * \param item Item das dargestellt werden soll
		 * \param player Spieler der das Item besitzt (bei Haendlern der Handelnde Spieler)
		 * \param gold Wenn nicht auf -1 gesetzt, wird geprueft, ob der Spieler sich das Item leisten kann
		 */
		void updateItemWindowTooltip(CEGUI::Window* img, Item* item, Player* player, int gold=-1);
		
		/**
		 * \fn void updateItemWindow(CEGUI::Window* img, Item* item, Player* player, int gold=-1)
		 * \brief aktualisiert das Bild eines Items
		 * \param img CEGUI Fenster
		 * \param item Item das dargestellt werden soll
		 * \param player Spieler der das Item besitzt (bei Haendlern der Handelnde Spieler)
		 * \param gold Wenn nicht auf -1 gesetzt, wird geprueft, ob der Spieler sich das Item leisten kann
		 */
		void updateItemWindow(CEGUI::Window* img, Item* item, Player* player, int gold=-1);
		
		/**
		 * \fn bool onItemHover(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Eintritt der Maus in Bereich ueber einem Item
		 */
		bool onItemHover(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onItemMouseButtonPressed(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Druecken der linken Maustaste ueber einen Item
		 */
		bool onItemMouseButtonPressed(const CEGUI::EventArgs& evt);

		/**
		 * \fn bool onItemMouseButtonReleased(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Loslassen der linken Maustaste ueber einen Item
		 */
		bool onItemMouseButtonReleased(const CEGUI::EventArgs& evt);
		
		
		/**
		 * \var static std::map<Item::Subtype, std::string> m_item_images
		 * \brief Enthaelt fuer jeden Itemtyp das passende Bild
		 */
		static std::map<Item::Subtype, std::string> m_item_images;
		
		
};

#endif
