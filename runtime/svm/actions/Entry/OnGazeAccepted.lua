local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "VR"
local group = "Entry"
local template = "OnGazeAccepted"

local image = "Entry/Entry.png"

local prop_trigger = "trigger"

local label_trigger = {en_US="Gaze", zh_CN="视线", tw="視線", pt="Olhar", fr="Regard", it="Sguardo", sp="Mirada", jp="視線", kr="시선", de="Blick", ru="Посмотреть"}
local txt_accepted = {en_US="Accepted", zh_CN="被触发时", tw="被觸發時", pt="Aceitaram", fr="Accepté", it="Accettato", sp="Aceptado", jp="受け入れ済み", kr="수락 됨", de="Akzeptiert", ru="Принято"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_trigger, "")

diagram.blockUiAddText(group, template, core.translate(label_trigger))
diagram.blockUiBindProperty(group, template, prop_trigger)
diagram.blockUiAddText(group, template, core.translate(txt_accepted))

diagram.inspectorUiBindLineEdit(group, template, prop_trigger, core.translate(label_trigger))
