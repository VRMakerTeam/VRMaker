local core = require "core"
local diagram = require "diagram"

local alias = {en_US="Camera", zh_CN="摄像机", tw="攝影機", pt="Câmera", fr="Caméra", it="telecamera", sp="Cámara", jp="カメラ", kr="카메라", de="Kamera", ru="камера"}

diagram.setGroupAlias("Camera", core.translate(alias))
