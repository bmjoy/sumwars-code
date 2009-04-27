#ifndef LISTITEM_H
#define LISTITEM_H

/**
 * \class ListItem
 * \brief Eintrag einer CEGUI Auswahlliste
 */
class ListItem : public CEGUI::ListboxTextItem
{
	public:
		ListItem(const CEGUI::String& text, uint id=0) : ListboxTextItem(text,id)
		{
			setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
			setAutoDeleted (true);
		}
};


/**
 * \class StrListItem
 * \brief Eintrag der Auswahlliste der Savegames
 */
class StrListItem : public ListItem
{
	public:
		StrListItem(const CEGUI::String& text, std::string data, uint id=0) : ListItem(text,id)
		{
			setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
			m_data = data;
		}

		std::string m_data;
};

#endif
