/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef FLUO_UI_MANAGER_HPP
#define FLUO_UI_MANAGER_HPP

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/python/dict.hpp>

#include <list>
#include <queue>
#include <map>

#include <typedefs.hpp>
#include <misc/config.hpp>

namespace fluo {
namespace world {
    class IngameObject;
}

namespace ui {

class CursorManager;
class GumpMenu;
class FontEngine;
class ShaderManager;
class ClipRectManager;
class UoFont;
class AudioManager;
class CommandManager;
class MacroManager;

namespace python {
class ScriptLoader;
}

namespace components {
class WorldView;
}

class Manager {
public:
    static bool create();
    static void destroy();
    static Manager* getSingleton();

    static const unsigned int TEXTURE_GROUP_WIDTH = 2048;
    static const unsigned int TEXTURE_GROUP_HEIGHT = 2048;

    bool setShardConfig(Config& config);

    static CL_GraphicContext& getGraphicContext();
    static CL_InputContext& getInputContext();
    static boost::shared_ptr<CL_DisplayWindow> getMainWindow();

    CL_Subtexture provideTexture(unsigned int usage, const CL_Size& size);
    void freeTexture(unsigned int usage, CL_Subtexture& texture);
    CL_Texture providerRenderBufferTexture(const CL_Size& size, CL_TextureFormat format = cl_rgba);

    static boost::shared_ptr<CL_GUIManager> getGuiManager();
    static boost::shared_ptr<CursorManager> getCursorManager();
    static boost::shared_ptr<FontEngine> getFontEngine();
    static boost::shared_ptr<ShaderManager> getShaderManager();
    static boost::shared_ptr<ClipRectManager> getClipRectManager();
    static boost::shared_ptr<AudioManager> getAudioManager();
    static boost::shared_ptr<CommandManager> getCommandManager();
    static boost::shared_ptr<MacroManager> getMacroManager();
    static boost::shared_ptr<python::ScriptLoader> getPythonLoader();

    static CL_Font getFont(const CL_FontDescription& desc);
    static UoFont& getUnifont(unsigned int index);

    static bool isMapIdIgnored(unsigned int id);
    static bool isStaticIdIgnored(unsigned int id);
    static bool isMapIdWater(unsigned int id);
    static bool isStaticIdWater(unsigned int id);

    void stepInput(unsigned int elapsedMillis);
    void stepAudio();
    void stepDraw();

    void closeGumpMenu(const UnicodeString& name);
    void closeGumpMenu(GumpMenu* menu);
    void closeGumpMenuByTypeId(unsigned int typeId);
    void destroyAllGumpMenus(); // needed for final cleanup
    void closeAllNonMessageGumps(); // called on disconnect

    std::list<GumpMenu*>::iterator gumpsBegin();
    std::list<GumpMenu*>::iterator gumpsEnd();
    // may only be called by the python loader!
    void processGumpCloseList();

    GumpMenu* getGumpMenu(const UnicodeString& name);

    void registerGumpMenu(GumpMenu* menu);

    void installMacros();
    void uninstallMacros();

    void onSingleClick(boost::shared_ptr<world::IngameObject> obj);
    void onDoubleClick(boost::shared_ptr<world::IngameObject> obj);
    void onDragDrop(boost::shared_ptr<world::IngameObject> dragObj, boost::shared_ptr<world::IngameObject> dragTarget, int locX = -1, int locY = -1);

    bool onUnhandledInputEvent(const CL_InputEvent& event);

    void queueComponentResize(CL_GUIComponent* comp, const CL_Rectf& geom);
    void queueComponentRepaint(CL_GUIComponent* comp);

    void releaseIngameObjects();

    CL_Point getMousePosition() const;

    void setWorldView(components::WorldView* view);
    components::WorldView* getWorldView() const;
    void showSpeechEntry(bool clearText = false);

    void setTheme(const UnicodeString& themeName);
    const boost::filesystem::path& getThemePath() const;

    void setPrompt(Serial serial);
    bool hasPrompt() const;
    void handlePrompt(const UnicodeString& text);
    void cancelPrompt();

    void openPythonGump(const UnicodeString& name);
    void openPythonGump(const UnicodeString& name, boost::python::dict& args);

    void handleTextInput(const UnicodeString& text);

    // save/load the open gumps
    void saveDesktop();
    void restoreDesktop();

    void setMouseOverObject(const boost::shared_ptr<world::IngameObject>& obj);

private:
    static Manager* singleton_;

    Manager();
    ~Manager();
    Manager(const Manager& copy) { }
    void operator=(const Manager& copy) { }

    // clan lib setup stuff
    CL_SetupCore clSetupCore;
    CL_SetupDisplay clSetupDisplay;
    CL_SetupGL clSetupGL;
    CL_SetupGUI clSetupGUI;

    // clan lib gui stuff
    boost::shared_ptr<CL_GUIManager> guiManager_;
    boost::shared_ptr<CL_GUIWindowManagerTexture> windowManager_;
    boost::shared_ptr<CL_DisplayWindow> mainWindow_;

    boost::shared_ptr<CursorManager> cursorManager_;

    std::list<GumpMenu*> gumpNewList_;
    std::list<GumpMenu*> gumpList_;
    std::list<GumpMenu*> gumpCloseList_;
    void processGumpNewList();

    CL_Slot slotCloseWindow;

    void loadFontDirectory(const boost::filesystem::path& path);
    void loadUnifonts();

    boost::shared_ptr<FontEngine> fontEngine_;

    boost::shared_ptr<ShaderManager> shaderManager_;

    UnicodeString getOpenGLExtensions() const;
    UnicodeString getOpenGLVendor() const;
    UnicodeString getOpenGLRenderer() const;

    boost::shared_ptr<ClipRectManager> clipRectManager_;

    boost::shared_ptr<AudioManager> audioManager_;

    boost::shared_ptr<CommandManager> commandManager_;
    boost::shared_ptr<MacroManager> macroManager_;

    std::map<unsigned int, CL_TextureGroup> textureGroups_;

    void onInputOutsideWindows(const CL_InputEvent& event, const CL_InputState& state);

    unsigned int doubleClickTimeout_;
    std::pair<boost::weak_ptr<world::IngameObject>, unsigned int> singleClickWait_;

    std::vector<int> mapIgnoreIds_;
    std::vector<int> staticIgnoreIds_;
    std::vector<int> mapWaterIds_;
    std::vector<int> staticWaterIds_;

    std::vector<std::pair<CL_GUIComponent*, CL_Rectf> > componentResizeQueue_;
    std::vector<CL_GUIComponent*> componentRepaintQueue_;

    components::WorldView* worldView_;

    boost::filesystem::path themePath_;

    Serial promptSerial_;

    boost::shared_ptr<python::ScriptLoader> pythonLoader_;

    boost::weak_ptr<world::IngameObject> mouseOverObject_;
    unsigned int clilocPropertiesTimeout_;
    unsigned int clilocPropertiesTimer_;
};

}
}

#endif
