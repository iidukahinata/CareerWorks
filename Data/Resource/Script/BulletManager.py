import Engine
import MonoBehaviour

class BulletManager(MonoBehaviour.MonoBehaviour) :

	def Start(self) :
		self.bulletLis = []
		for i in range(100) :
			gameobject = self.world.CreateGameObject(self.scene)
			gameobject.transform.SetParent(self.transform)
			self.bulletLis.append(gameobject)

		return