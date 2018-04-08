local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "basic"
local group = "Exit"
local template = "InvokeCommand"

local image = "Exit/Exit.png"

local prop_command = "command"

local label_command = {en_US="Command", zh_CN="命令"}
local txt_invoke = {en_US="Invoke", zh_CN="调用"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_command, "")

diagram.blockUiAddText(group, template, core.translate(txt_invoke))
diagram.blockUiBindProperty(group, template, prop_command)

diagram.inspectorUiBindLineEdit(group, template, prop_command, core.translate(label_command))
