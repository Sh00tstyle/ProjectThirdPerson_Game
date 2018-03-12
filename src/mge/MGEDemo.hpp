#ifndef MGEDEMO_HPP
#define MGEDEMO_HPP

#include "mge/core/AbstractGame.hpp"
#include "mge/eventSystem/SystemEventDispatcher.hpp"

class Mesh; 
class AbstractMaterial; 
class DebugHud;
class Mesh;
class AbstractMaterial;
class SceneManager;
class ModelManager;
class UiContainer;
class AudioContainer;

/**
 * An example subclass of AbstractGame showing how we can setup a scene.
 */
class MGEDemo: public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		MGEDemo();
		virtual ~MGEDemo();

        //override initialize so we can add a DebugHud
        virtual void initialize();

		static void UpdateLoadingScreen(int percentage);

	protected:
	    //override so we can construct the actual scene
        virtual void _initializeScene();

		virtual void _initializeMenu();

	    //override render to render the hud as well.
		virtual void _renderUi();

		//override, to add own keypress events
		virtual void _processEvents();

	private:
		DebugHud* _hud;                   //hud display

		float _screenRatio;
		float _orthoSize;

		ModelManager* _modelManager;
		SceneManager* _sceneManager;
		AudioContainer* _audioContainer;

		static UiContainer* _uiContainer;

        void _updateHud();

        MGEDemo(const MGEDemo&);
        MGEDemo& operator=(const MGEDemo&);
};

#endif // MGEDEMO_HPP
