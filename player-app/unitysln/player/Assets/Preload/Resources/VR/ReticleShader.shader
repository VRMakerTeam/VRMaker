// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'


Shader "VRXX/Reticle" {
  Properties {
    _Color  ("Color", Color) = ( 1, 1, 1, 1 )
    _InnerDiameter ("InnerDiameter", Range(0, 10.0)) = 0.15
    _OuterDiameter ("OuterDiameter", Range(0.00872665, 10.0)) = 0.2
    _DistanceInMeters ("DistanceInMeters", Range(0.0, 100.0)) = 15.0
  }

  SubShader {
    Tags { "Queue"="Overlay" "IgnoreProjector"="True" "RenderType"="Transparent" }
    Pass {
      Blend SrcAlpha OneMinusSrcAlpha
      AlphaTest Off
      Cull Back
      Lighting Off
      ZWrite Off
      ZTest Always

      Fog { Mode Off }
      CGPROGRAM

      #pragma vertex vert
      #pragma fragment frag

      #include "UnityCG.cginc"

      uniform float4 _Color;
      uniform float _InnerDiameter;
      uniform float _OuterDiameter;
      uniform float _DistanceInMeters;

      struct vertexInput {
        float4 vertex : POSITION;
      };

      struct fragmentInput{
          float4 position : SV_POSITION;
      };

      fragmentInput vert(vertexInput i) {
        float scale = lerp(_OuterDiameter, _InnerDiameter, i.vertex.z);

        float4 vert_out = float4(i.vertex.x * scale, i.vertex.y * scale, _DistanceInMeters, 1.0);

        fragmentInput o;
        o.position = UnityObjectToClipPos (vert_out);
        return o;
      }

      fixed4 frag(fragmentInput i) : SV_Target {
        fixed4 ret = fixed4(_Color.x, _Color.y, _Color.z, 1.0);
        return ret;
      }

      ENDCG
    }
  }
}
