import Engine
import MonoBehaviour

class testScript(MonoBehaviour.MonoBehaviour) :

    def Start(self) :
        return
    
    def Update(self, deltaTime) :
        self.transform.rotation.y += 0.01
        return
    
    def OnKeyPressed(self, data) :
        if data == Engine.KeyAndMouse.A :
            self.transform.position.x += 1.0
        return