local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "basic"
local group = "Time"
local template = "Sleep"

local image = "Time/Time.png"

local prop_time = "duration"
local label_time = {en_US="Time", zh_CN="时间", tw="時間", pt="Tempo", fr="Temps", it="Tempo", sp="Hora", jp="時間", kr="시각", de="Zeit", ru="Время"}
local txt_sleep = {en_US="Wait", zh_CN="等待", zh_TW="等待", pt="Esperar", fr="Attendre", it="Aspettare", sp="Espere", jp="待つ", kr="기다림", de="Warten", ru="Подождите"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_time,0) 

diagram.blockUiAddText(group, template, core.translate(txt_sleep))

diagram.inspectorUiBindNumberSpin(group, template, prop_time, core.translate(label_time), 0, 999999.0)
