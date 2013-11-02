/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "OgreResourceGroupManager.h"
#include "OgreTextureManager.h"
#include "savegamelist.h"
#include "stdstreamconv.h"

#ifdef SUMWARS_BUILD_WITH_ONLINE_SERVICES
#include "onlineservicesmanager.h"
#endif

// Utility for CEGUI cross-version compatibility
#include "ceguiutility.h"

// needed to be able to create the CEGUI renderer interface
#ifdef CEGUI_07
#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"
#else
#include "CEGUI/RendererModules/Ogre/Renderer.h"
#endif

// Sound operations helper.
#include "soundhelper.h"


SavegameList::SavegameList (Document* doc, const std::string& ceguiSkinName)

	: Window(doc)
	, m_ceguiSkinName (ceguiSkinName)
{
	DEBUGX("setup main menu");
	// GUI Elemente erzeugen

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	m_currentSelected = 0;

	// Rahmen fuer das Menue Savegame auswaehlen
	CEGUI::FrameWindow* save_menu = (CEGUI::FrameWindow*) win_mgr.createWindow (CEGUIUtility::getWidgetWithSkin (m_ceguiSkinName, "FrameWindow"), "SavegameMenu");
	save_menu->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f))); //0.0/0.8
	CEGUIUtility::setWidgetSizeRel (save_menu, 1.0f, 1.0f);
	save_menu->setProperty("FrameEnabled","false");
	save_menu->setProperty("TitlebarEnabled","false");
	save_menu->setProperty("CloseButtonEnabled","false");
	save_menu->setAlpha(0.0f);
	save_menu->setInheritsAlpha(false);
	
	m_window = save_menu;

	// Bestandteile der Kontrollleiste hinzufuegen
	CEGUI::PushButton* btn;
	//CEGUI::Window* label;// detected as unused, 2011.10.23.

	
	// Button neu
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow (CEGUIUtility::getWidgetWithSkin (m_ceguiSkinName, "Button"), "NewCharButton"));
	CEGUIUtility::addChildWidget (save_menu, btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.85f)));
	CEGUIUtility::setWidgetSizeRel (btn, 0.4f, 0.05f);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SavegameList::onNewCharClicked, this));
	btn->subscribeEvent(CEGUIUtility::EventMouseEntersPushButtonArea (), CEGUI::Event::Subscriber(&SavegameList::onItemButtonHover, this));
	btn->setWantsMultiClickEvents(false);
	btn->setInheritsAlpha(false);
	if (btn->isPropertyPresent ("NormalImage"))
	{
		btn->setProperty("NormalImage", "set:MainMenu image:SPBtnNormal");
	}
	if (btn->isPropertyPresent ("HoverImage"))
	{
		btn->setProperty("HoverImage", "set:MainMenu image:SPBtnHover");
	}
	if (btn->isPropertyPresent ("PushedImage"))
	{
		btn->setProperty("PushedImage", "set:MainMenu image:SPBtnPushed");
	}
	
    m_numCurrentCharacterButtons = 0;

	updateTranslation();
}

void SavegameList::update()
{	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	// Liste aller Files im Save Ordner der Form *.sav
	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;

#ifdef SUMWARS_BUILD_WITH_ONLINE_SERVICES
    if(!OnlineServicesManager::getSingleton().userLoggedIn())
        files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("Savegame","*.sav");
    else
    {
        std::string id = OnlineServicesManager::getSingleton().getUserDataResGroupId();
        files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(id,"*.sav");
    }
#else
    files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("Savegame","*.sav");
#endif


	std::fstream file;
	char bin;
	int n = 0;
	char lev;
	
	std::string name;
	std::string classname;
	std::ostringstream stream;
	std::string filename;
	PlayerLook look;
	int version;
	// iterieren ueber die Files
	unsigned char* data;

    for(int i = 0; i < m_numCurrentCharacterButtons; i++)
    {
		std::ostringstream s;
		s << i;

        CEGUI::Window* saveItem = 0;
        try
        {
            saveItem = win_mgr.getWindow(s.str().append("SaveItemRoot"));
			saveItem->hide();
            //m_window->removeChildWindow(saveItem);
        }
        catch(CEGUI::UnknownObjectException&)
        {
        }
    }

    m_numCurrentCharacterButtons = files->size();

	float height = m_window->getPixelSize().d_width / 4.0f;

	for (it = files->begin(); it!= files->end();++it)
    {
		filename = it->archive->getName();
		filename += "/";
		filename += it->filename;
		DEBUGX("file found %s",filename.c_str());
		//File oeffnen

		file.open(filename.c_str(),std::ios::in| std::ios::binary);
		if (file.is_open())
		{
			std::ostringstream s;
			s << n;
			bool needToLoadLayout = false;

			CEGUI::Window* saveItem = 0;
			try
			{
				saveItem = CEGUIUtility::getWindow (s.str ().append ("SaveItemRoot"));
				if (!saveItem)
					needToLoadLayout = true;
			}
			catch(CEGUI::UnknownObjectException&)
			{
				needToLoadLayout = true;
			}

			if (needToLoadLayout)
			{
				saveItem = (CEGUI::Window*) win_mgr.loadWindowLayout("saveitem.layout", s.str());
				m_currentSelected = saveItem;
				saveItem->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SavegameList::onSavegameChosen, this));
				saveItem->subscribeEvent(CEGUI::PushButton::EventMouseEnters, CEGUI::Event::Subscriber(&SavegameList::onItemButtonHover, this));
				m_window->addChildWindow(saveItem);
				saveItem->show();

				// Store the mapping entry;
				m_fileSaveMapping [s.str().append("SaveItemRoot/DelChar")] = it->filename;
				
				// make buttons resolution independant
				saveItem->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_absdim((height + 2.0f)*n)));
				CEGUIUtility::setWidgetSize (saveItem, CEGUI::UVector2(cegui_reldim(1.0f), cegui_absdim(height)));
				saveItem->getChild(s.str().append("SaveItemRoot/Name"))->setMousePassThroughEnabled(true);
				saveItem->getChild(s.str().append("SaveItemRoot/DecriptionLabel"))->setMousePassThroughEnabled(true);
				saveItem->getChild(s.str().append("SaveItemRoot/Avatar"))->setMousePassThroughEnabled(true);
				saveItem->getChild(s.str().append("SaveItemRoot/DelChar"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SavegameList::onDeleteCharClicked, this));	
				saveItem->getChild(s.str().append("SaveItemRoot/DelChar"))->subscribeEvent (CEGUIUtility::EventMouseEntersPushButtonArea (), CEGUI::Event::Subscriber(&SavegameList::onItemButtonHover, this));
			}
			else
			{
				saveItem->show();
				m_currentSelected = saveItem;
				// Store the file name in a mapping, along with this widget name
				m_fileSaveMapping [s.str().append("SaveItemRoot")] = it->filename;
			}
			
			// set the proper Image
			std::string texName = filename.erase(filename.length() - 4, filename.length());
			Ogre::TextureManager *tmgr = Ogre::TextureManager::getSingletonPtr();

			int pos = texName.find_last_of("\\");
			if (texName.find_last_of("/") > (size_t)pos)
			{
				pos = texName.find_last_of("/");
			}

			std::string nameNoPath = texName.erase(0, pos+1).append(".png");
			CEGUI::String imagesetName = (CEGUI::utf8*) (texName + "SaveItemRootAvatarImageset").c_str();
			
			CEGUI::String imageName = CEGUI::String("set:") + imagesetName + "  " + "image:MainMenuAvatarImg";
			
			DEBUGX("imagename %s",imageName.c_str() );
			DEBUGX("imagesetName %s", imagesetName.c_str());
			
			// TODO: need to check if the image in the imageset is defined, not just the imageset.
			
			if (CEGUIUtility::getImageManager ().isDefined(imagesetName))
			{
				saveItem->getChild(s.str().append("SaveItemRoot/Avatar"))->setProperty("Image", imageName);
			}
			else
			{
                std::string resGrp = "Savegame";

#ifdef SUMWARS_BUILD_WITH_ONLINE_SERVICES
                if(OnlineServicesManager::getSingletonPtr())
                    resGrp = OnlineServicesManager::getSingleton().getUserDataResGroupId();
#endif

				// create CEGUI texture for the character thumbnail
				if(Ogre::ResourceGroupManager::getSingleton().resourceExists(resGrp, nameNoPath))
				{
					CEGUI::String textureName ("MainMenuAvatar_Tex");
					Ogre::TexturePtr tex = tmgr->load(texName, "Savegame");

#ifdef CEGUI_07
					CEGUI::Texture &ceguiTex = static_cast<CEGUI::OgreRenderer*>
						(CEGUI::System::getSingleton().getRenderer())->createTexture(tex);


					CEGUI::Imageset& textureImageSet = CEGUIUtility::getImageManager ().create (imagesetName, ceguiTex);
					textureImageSet.defineImage( "MainMenuAvatarImg",
								CEGUIUtility::Vector2f (0.0f, 0.0f),
								CEGUIUtility::Size (ceguiTex.getSize().d_width, ceguiTex.getSize().d_height),
								CEGUIUtility::Vector2f (0.0f, 0.0f)
								);
#else
					CEGUI::Texture &ceguiTex = static_cast<CEGUI::OgreRenderer*>
						(CEGUI::System::getSingleton ().getRenderer ())->createTexture (textureName, tex);

					CEGUI::OgreRenderer* rendererPtr = static_cast<CEGUI::OgreRenderer*>(CEGUI::System::getSingleton().getRenderer());
					CEGUI::String imageName("MainMenuAvatarImg");
					CEGUI::TextureTarget*   d_textureTarget;
					CEGUI::BasicImage*      d_textureTargetImage;
					d_textureTarget = rendererPtr->createTextureTarget();
					d_textureTargetImage = static_cast<CEGUI::BasicImage*>(&CEGUI::ImageManager::getSingleton().create("BasicImage", imageName));
					d_textureTargetImage->setTexture(&ceguiTex);
					d_textureTargetImage->setArea(CEGUI::Rectf(0, 0, ceguiTex.getSize ().d_width,ceguiTex.getSize ().d_height));
#endif
					saveItem->getChild(s.str().append("SaveItemRoot/Avatar"))->setProperty("Image", imageName);
				}
				else
				{
					saveItem->getChild(s.str().append("SaveItemRoot/Avatar"))->setProperty("Image","set:MainMenu image:Logo");
				}
			}

			file.get(bin);
			
			CharConv* save;
			data =0;
			if (bin == '0')
			{
				save = new StdStreamConv(&file);
			}
			else
			{
				// binary Savegames are not supported anymore
				file.close();
				continue;
			}
			
			save->fromBuffer(version);
			save->setVersion(version);
			
			save->fromBuffer(classname);
			save->fromBuffer(name);
			look.fromString(save);
			save->fromBuffer(lev);
			
			stream.str("");
			stream << (int) lev;


			CEGUI::String temp;
			temp.append((CEGUI::utf8*) gettext("Level")).append(" ").append(stream.str()).append(" ").append((CEGUI::utf8*) gettext(classname.c_str()));
			saveItem->getChild(s.str().append("SaveItemRoot/Name"))->setText((CEGUI::utf8*) name.c_str());
			saveItem->getChild(s.str().append("SaveItemRoot/DecriptionLabel"))->setText(temp);

			n++;

			file.close();
			
			delete save;
			if (data != 0)
				 delete data;
        }
	}
	
	CEGUI::PushButton *btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow("NewCharButton"));
	if (btn)
	{
		btn->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_absdim((height + 2.0f)*n+1)));
		CEGUIUtility::setWidgetSize (btn, CEGUI::UVector2(cegui_reldim(1.0f), cegui_absdim(height)));
	}
}

void SavegameList::selectDefaultSavegame()
{
	std::string savegame = Options::getInstance()->getDefaultSavegame();
	if (savegame != "")
	{
		m_document->setSaveFile(savegame.c_str());
	}
}

void SavegameList::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	/*
	CEGUI::Window* label;
	label = win_mgr.getWindow("SavegameChooseLabel");
	label->setText((CEGUI::utf8*) gettext("Characters"));*/
	
	/*btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("SelectSavegameButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));*/
	
	btn = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow("NewCharButton"));
	if (btn)
	{
		btn->setText((CEGUI::utf8*) gettext("Create"));
	}
}


/**
* \fn bool onItemButtonHover(const CEGUI::EventArgs& evt)
* \brief Handle the hovering of a menu item
*/
bool SavegameList::onItemButtonHover(const CEGUI::EventArgs& evt)
{
	SoundHelper::playAmbientSoundGroup ("main_menu_hover_item");
	return true;
}


bool SavegameList::onSavegameChosen(const CEGUI::EventArgs& evt)
{
	const CEGUI::WindowEventArgs& we = static_cast<const CEGUI::WindowEventArgs&>(evt);
	
	std::string prefix = we.window->getName().c_str();
	std::string saveWidgetName = prefix;

	prefix.erase(prefix.length()-12, prefix.length());

	std::string name = we.window->getChild(prefix.append("SaveItemRoot/Name"))->getText().c_str();

	// Get the file mapped to this widget: if we have a mapping, use the mapped name, otherwise, just add an extension and try to use the same file name as the char name.
	std::map <std::string, std::string>::const_iterator it = m_fileSaveMapping.find (saveWidgetName);
	if (it != m_fileSaveMapping.end ())
	{
		name = m_fileSaveMapping [saveWidgetName];
		DEBUG ("got mapping for [%s] as [%s]", saveWidgetName.c_str (), name.c_str ());
	}
	else
	{
		name.append(".sav");
	}

	m_document->setSaveFile(name.c_str());
	DEBUGX("selected Savegame %s", sitm->m_data.c_str());

	// Also play the click sound.
	SoundHelper::playAmbientSoundGroup ("main_menu_click_item");

	return true;
}

bool SavegameList::onSavegameSelected(const CEGUI::EventArgs& evt)
{
	if (m_document->getSaveFile() != "")
	{
		DEBUG("savegame accepted %s", m_document->getSaveFile().c_str());
		m_document->getGUIState()->m_shown_windows = Document::START_MENU;
		m_document->setModified(Document::WINDOWS_MODIFIED);
	}
	return true;
}

bool SavegameList::onNewCharClicked(const CEGUI::EventArgs& evt)
{
	// Also play the click sound.
	SoundHelper::playAmbientSoundGroup ("main_menu_click_item");

	m_document->onCreateNewCharButton();
	return true;
}


bool SavegameList::onDeleteCharClicked(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	const CEGUI::WindowEventArgs& we = static_cast<const CEGUI::WindowEventArgs&>(evt);
	onSavegameChosen(CEGUI::WindowEventArgs(we.window->getParent()));
	
	// Also play the notification sound.
	SoundHelper::playAmbientSoundGroup ("main_menu_notification");

	m_document->showQuestionDialog ();

	return true;
}

bool SavegameList::onDeleteCharConfirmClicked(const CEGUI::EventArgs& evt)
{
	// Also play the click sound.
	SoundHelper::playAmbientSoundGroup ("main_menu_click_item");

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	m_document->hideQuestionDialog ();

	// Get the save file to remove.
	std::string saveFile =  m_document->getSaveFile();
	if (saveFile.empty())
	{
		return false;
	}
	
	// Get rid of the save file
	remove (saveFile.c_str ());

	// Also get rif od the preview image.
	// The preview image should have the same name as the save file, but with a different extension.
	// Using the standard 3 char extension (4 if the "." is included).
	if (saveFile.length () > 4)
	{
		// Parse the name.
		std::string previewPicture (saveFile);
		previewPicture.erase (saveFile.length () - 4, saveFile.length());
		previewPicture.append (".png");

		// Get rid of the file
		remove (previewPicture.c_str ());
	}

	// Clear the selection in the menu.
	if(m_currentSelected != 0)
	{
		CEGUI::WindowManager::getSingleton().destroyWindow(m_currentSelected);
		m_currentSelected = 0;
	}
	m_document->setSaveFile("");
	
	update();
	return true;
}

bool SavegameList::onDeleteCharAbortClicked(const CEGUI::EventArgs& evt)
{
	// Also play the click sound.
	SoundHelper::playAmbientSoundGroup ("main_menu_click_item");

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	m_document->hideQuestionDialog ();

	return true;
}

bool SavegameList::onSavegameDoubleClick(const CEGUI::EventArgs& evt)
{
	// Also play the click sound.
	SoundHelper::playAmbientSoundGroup ("main_menu_click_item");

	onSavegameSelected(evt);
	return true;
}
