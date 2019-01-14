local core = require "core"
local log = require "log"
local diagram = require "diagram"
local canvas = require "canvas"

local extend = "basic"
local group = "Camera"
local template = "Shake"

local image = "Camera/Camera.png"

local prop_duration = "duration"

local label_duration = {en_US="Duration", zh_CN="时间", tw="時間", pt="Duração", fr="Durée", it="Durata", sp="Duración", jp="期間", kr="지속", de="Dauer", ru="продолжительность"} 
local txt_shake = {en_US="Shake", zh_CN="抖动", tw="抖動", pt="Mexe", fr="Secouer", it="Agitare", sp="Sacudir", jp="振る", kr="떨림", de="Shake", ru="Встряска"}


log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_duration, 0.0)

--
-- Block UI
--
diagram.blockUiAddText(group, template, core.translate(txt_shake))

--
-- Inspector UI
-- 
diagram.inspectorUiBindNumberSpin(group, template, prop_duration, core.translate(label_duration), 0, 9999999)

