local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "basic"
local group = "Entry"
local template = "OnCommandInvoke"

local image = "Entry/Entry.png"

local prop_command = "command"

local label_command = {en_US="Command", zh_CN="命令"}
local txt_invoked = {en_US="Invoked", zh_CN="被调用时"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_command, "")

diagram.blockUiAddText(group, template, core.translate(label_command))
diagram.blockUiBindProperty(group, template, prop_command)
diagram.blockUiAddText(group, template, core.translate(txt_invoked))

diagram.inspectorUiBindLineEdit(group, template, prop_command, core.translate(label_command))
