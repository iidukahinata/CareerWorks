import Engine

def Start(this) :
    return

def Update(this, deltaTime) :
    rot = this.transform.rotation
    rot.y += 0.01
    this.transform.rotation = rot
    return

def OnKeyPressed(this, data) :
    pos = this.transform.position

    if data == Engine.KeyAndMause.A :
        pos.x += 1.0

    this.transform.position = pos
    return