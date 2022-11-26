import Engine
import MonoBehaviour

class testScript(MonoBehaviour.MonoBehaviour) :
    def Start(self) :
        return
    
    def Update(self, deltaTime) :
        rot = self.transform.rotation
        rot.y += 0.01
        self.transform.rotation = rot
        return
    
    def OnKeyPressed(self, data) :
        #pos = this.transform.position
        #if data == Engine.KeyAndMause.A :
        #    pos.x += 1.0
        #this.transform.position = pos
        return