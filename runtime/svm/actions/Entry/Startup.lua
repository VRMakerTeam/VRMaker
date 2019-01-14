local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "basic"
local group = "Entry"
local template = "Startup"

local label = {en_US="Run", zh_CN="开始", tw="開始", pt="Começar", fr="Début", it="Inizio", sp="comienzo", jp="開始", kr="스타트", de="Lauf", ru="Начать бег"}

local image = "Entry/Entry.png"

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockUiAddText(group, template, core.translate(label))
