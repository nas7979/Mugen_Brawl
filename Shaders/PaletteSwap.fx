texture gMainTex;
texture gPaletteTex;
float gAlpha;

sampler2D MainTex = sampler_state
{
    Texture = (gMainTex);
};

sampler2D PaletteTex = sampler_state
{
    Texture = (gPaletteTex);
};

float4 PS_Main(float2 uv : TEXCOORD0) : COLOR0
{
    float4 diffuse = tex2D(MainTex, uv);
    int index = diffuse.r * 255;
    float4 paletteColored = tex2D(PaletteTex, float2(index % 32, index / 32) / 32);
    
    return float4(paletteColored.r, paletteColored.g, paletteColored.b, gAlpha * paletteColored.a);
}

technique
{
    pass p0
    {
        AlphaBlendEnable = TRUE;
        DestBlend = INVSRCALPHA;
        SrcBlend = SRCALPHA;
        PixelShader = compile ps_2_0 PS_Main();
    }
}