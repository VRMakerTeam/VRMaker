local core = require "core"
local log = require "log"
local diagram = require "diagram"
local canvas = require "canvas"

local extend = "basic"
local group = "Camera"
local template = "SetFOV"

local image = "Camera/Camera.png"

local prop_fov = "fov"

local label_fov = "FOV"

local txt_setfov = {en_US="SetFOV", zh_CN="设置视角"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_fov, 0) 

--
-- Block UI
--
diagram.blockUiAddText(group, template, core.translate(txt_setfov)) 

--
-- Inspector UI
-- 
diagram.inspectorUiBindIntegerSpin(group, template, prop_fov, label_fov, 1, 179)

