local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "basic"
local group = "Exit"
local template = "SwitchSlate"

local image = "Exit/Exit.png"

local prop_slate = "slate"

local label_slate = {en_US="Slate", zh_CN="分镜", tw="分鏡", pt="Ardósia", fr="Ardoise", it="Ardesia", sp="Pizarra", jp="スレート", kr="슬레이트", de="Schiefer", ru="шифер"}

local txt_switchslate = {en_US="Jump to cut scene", zh_CN="切换分镜", tw="切換分鏡", pt="Pule para cortar cena", fr="Aller à la scène", it="Vai a tagliare la scena", sp="Salta para cortar escena", jp="カットシーンへジャンプ", kr="장면 전환", de="Springe, um die Szene zu schneiden", ru="Перейти к сцене"}


log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_slate, "")

diagram.blockUiAddText(group, template, core.translate(txt_switchslate))
diagram.blockUiBindProperty(group, template, prop_slate)

diagram.inspectorUiBindLineEdit(group, template, prop_slate, core.translate(label_slate))
