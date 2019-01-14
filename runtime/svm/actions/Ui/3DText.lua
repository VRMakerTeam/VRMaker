local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "VR"
local group = "Ui"
local template = "New3DText"

local image = "Ui/Ui.png"

local txt_3DText = {en_US="3DText", zh_CN="3D文本"}

local prop_text = "text"
local prop_size = "size"
local prop_font_color = "font:color"
local prop_background_color = "background:color"
local prop_animation = "animation"

local prop_x = "posx"
local prop_y = "posy"
local prop_z = "posz"
local prop_w = "width"
local prop_h = "high"


local label_x = "X"
local label_y = "Y"
local label_z = "Z"
local label_w = "width"
local label_h = "high"
local label_text = {en_US="Text", zh_CN="文本", tw="文字", pt="texto", fr="texte", it="testo", sp="texto", jp="テキスト", kr="본문", de="Text", ru="Текст"}
local label_font_color = {en_US="FontColor", zh_CN="字体颜色", tw="字體顏色", pt="cor da fonte", fr="couleur de la police", it="colore del carattere", sp="color de fuente", jp="フォントの色", kr="글꼴 색상", de="Schriftfarbe", ru="Цвет шрифта"}
local label_bcakground_color = {en_US="BackGroundColor", zh_CN="背景顏色", tw="背景顏色", pt="cor de fundo", fr="Couleur de fond", it="colore di sfondo", sp="color de fondo", jp="バックグラウンドカラー", kr="배경색", de="Hintergrundfarbe", ru="Фоновый цвет"}
local label_size = {en_US="FontSize", zh_CN="字体大小", tw="字體大小", pt="tamanho da fonte", fr="taille de police", it="dimensione del font", sp="tamaño de fuente", jp="フォントサイズ", kr="글자 크기", de="Schriftgröße", ru="Размер шрифта"}
local label_animation = {en_US="Insert Animation", zh_CN="插入动画", tw="插入動畫", pt="Inserir animação", fr="Insérer une animation", it="Inserisci animazione", sp="Insertar animación", jp="アニメーションを挿入", kr="애니메이션 삽입", de="Animation einfügen", ru="Вставить анимацию"}
local item_null = {en_US="none", zh_CN="无", tw="沒有", pt="Não", fr="Non", it="No", sp="No", jp="無し", kr="없음", de="Keiner", ru="Никто"}
local item_fadein = {en_US="FadeIn", zh_CN="淡入", tw="淡入", pt="aparecimento gradual", fr="fondu", it="svanire", sp="fundirse", jp="フェードイン", kr="점점 뚜렷해지다", de="Ausblenden", ru="Исчезать" }
local item_pullin = {en_US="PullIn", zh_CN="拉近", tw="拉近", pt="Puxar", fr="Tirez dans", it="Tirare dentro", sp="Tirar de", jp="引き込む", kr="당겨", de="Einziehen", ru="Втянуть"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_text, "")
diagram.blockRegisterProperty(group, template, prop_size, 40)
diagram.blockRegisterProperty(group, template, prop_animation, "")
diagram.blockRegisterProperty(group, template, prop_x, 0.0)
diagram.blockRegisterProperty(group, template, prop_y, 0.0)
diagram.blockRegisterProperty(group, template, prop_z, 6.0)
diagram.blockRegisterProperty(group, template, prop_w, 200.0)
diagram.blockRegisterProperty(group, template, prop_h, 200.0)

--
-- Block UI
--
diagram.blockUiAddText(group, template, core.translate(txt_3DText)) 

--
-- Inspector UI
-- 
diagram.inspectorUiBindTextEdit(group, template, prop_text, core.translate(label_text))
diagram.inspectorUiBindIntegerSpin(group, template, prop_size, core.translate(label_size), 0, 300)
diagram.inspectorUiBindColorPicker(group, template, prop_font_color , core.translate(label_font_color), 255, 255, 255, 255)
diagram.inspectorUiBindColorPicker(group, template, prop_background_color, core.translate(label_bcakground_color), 0, 0, 0, 125)
diagram.inspectorUiBindComboBox(group, template, prop_animation, core.translate(label_animation), {core.translate(item_null), core.translate(item_fadein), core.translate(item_pullin)})
diagram.inspectorUiBindNumberSpin(group, template, prop_x, label_x, -999999999.0, 999999999.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_y, label_y, -999999999.0, 999999999.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_z, label_z, -999999999.0, 999999999.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_w, label_w, -999999999.0, 999999999.0)
diagram.inspectorUiBindNumberSpin(group, template, prop_h, label_h, -999999999.0, 999999999.0)

