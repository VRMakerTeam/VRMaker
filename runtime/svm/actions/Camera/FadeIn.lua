local core = require "core"
local log = require "log"
local diagram = require "diagram"
local canvas = require "canvas"

local extend = "basic"
local group = "Camera"
local template = "FadeIn"

local image = "Camera/Camera.png"

local prop_r = "color:r"
local prop_g = "color:g"
local prop_b = "color:b"
local prop_duration = "duration"

local label_r = "R"
local label_g = "G"
local label_b = "B"
local label_duration = {en_US="Duration", zh_CN="时间", tw="時間", pt="Duração", fr="Durée", it="Durata", sp="Duración", jp="期間", kr="지속", de="Dauer", ru="продолжительность"} 

local txt_fadein = {en_US="FadeIn", zh_CN="淡入", tw="淡入", pt="Aparecimento gradual", fr="Durée", it="FadeIn", sp="Fundirse", jp="フェードイン", kr="점점 뚜렷해지다", de="Ausblenden", ru="Исчезать"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_r, 0) 
diagram.blockRegisterProperty(group, template, prop_g, 0)
diagram.blockRegisterProperty(group, template, prop_b, 0)
diagram.blockRegisterProperty(group, template, prop_duration, 0.0)

--
-- Block UI
--
diagram.blockUiAddText(group, template, core.translate(txt_fadein))

--
-- Inspector UI
-- 
diagram.inspectorUiBindIntegerSpin(group, template, prop_r, label_r, 0, 255)
diagram.inspectorUiBindIntegerSpin(group, template, prop_g, label_g, 0, 255)
diagram.inspectorUiBindIntegerSpin(group, template, prop_b, label_b, 0, 255)
diagram.inspectorUiBindNumberSpin(group, template, prop_duration, core.translate(label_duration), 0, 9999999)

