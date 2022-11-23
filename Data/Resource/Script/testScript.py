import Engine

def Start(this) :
    return

def Update(this, deltaTime) :
    rot = this.transform.rotation
    rot.y += 0.01
    this.transform.rotation = rot
    return