local core = require "core"
local log = require "log"
local diagram = require "diagram"
local canvas = require "canvas"

local extend = "basic"
local group = "Camera"
local template = "SetPosition"

local image = "Camera/Camera.png"

local prop_x = "x"
local prop_y = "y"
local prop_z = "z"

local label_x = "x"
local label_y = "y"
local label_z = "z"

local txt_setposition = {en_US="Set Position", zh_CN="设置位置", tw="設定位置", pt="Definir posição", fr="Définir la perspective", it="Imposta posizione", sp="Posición de ajuste", jp="位置を設定", kr="위치 설정", de="setzen", ru="Установить положение"}

local txt_usesnap= {en_US="Snapshot", zh_CN="使用快照", tw="螢幕截圖", pt="Instantâneo", fr="Définir la perspective", it="istantanea", sp="Instantánea", jp="スナップショット", kr="스냅 사진", de="Schnappschuss", ru="снимок"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_x, 0.0) 
diagram.blockRegisterProperty(group, template, prop_y, 0.0)
diagram.blockRegisterProperty(group, template, prop_z, 0.0)

--
-- Block UI
--
diagram.blockUiAddText(group, template, core.translate(txt_setposition))

--
-- Inspector UI
-- 
local invoker_ = ""

function onInspectorBuildFinish(_params)
    invoker_ = _params.invoker
    log.debug("onInspectorBuildFinish, invoke is " .. invoker_)
end

function onButtonClick()
    log.trace("onButtonClick")
    local params = {}
    canvas.invokeMessage("JSQueryCameraPosition", invoker_, params)
end

diagram.inspectorBindCallback(group, template, "onBuildFinish", onInspectorBuildFinish)

diagram.inspectorUiBindNumberSpin(group, template, prop_x, label_x, -999999999.0, 999999999.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_y, label_y, -999999999.0, 999999999.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_z, label_z, -999999999.0, 999999999.0)
diagram.inspectorUiBindButton(group, template, core.translate(txt_usesnap), onButtonClick)


--
-- Canvas
--
function handleCanvasMessage(_message, _invoker, _params)
    if (_invoker ~= invoker_ )
    then
        return
    end

    if(_message == "JSOnQueryCameraPosition")
    then
        local props = {}
        props[prop_x] = _params.x
        props[prop_y] = _params.y
        props[prop_z] = _params.z
        log.debug("Handle JSOnQueryCameraPosition, x:" .. props[prop_x] .. " y:" .. props[prop_y] .. " z:" .. props[prop_z])
        diagram.inspectorUiUpdateProperty(_invoker, props)
    end
end

canvas.bindHandler(handleCanvasMessage)

