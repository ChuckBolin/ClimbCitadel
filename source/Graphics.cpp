/**********************************************************
 GRAPHICS.CPP - Written by Chuck Bolin
 Thanks to http://www.toymaker.info/Games/html/sprites.html
 for the tutorials on 2D sprites with DirectX

 Save sprites with ImageComposer as PNG, check Transparent
 color, set threshold to 127
 *********************************************************/
#include "..\includes\Graphics.h"
#include "..\includes\Log.h"
extern Log g_Log;

CDXGraphics::CDXGraphics(){
	for (int i = 0; i < GF_MAX_FILES; i++)
	{
    m_pFile[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name :	InitD3D () (Public)
// Desc :	Performs a simple, non-enumerated, initialization of Direct3D
//-----------------------------------------------------------------------------
int CDXGraphics::InitD3D (HWND hWnd, bool bWindowedMode)
{
  HRESULT               hRet;
  D3DCAPS9              Caps;
	D3DDISPLAYMODE        CurrentMode;
	m_bWindowed			= bWindowedMode;

	m_TextureCount = 0;

  // First of all create our D3D Object
  m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
  if (!m_pD3D) 
  {
    MessageBox( hWnd, "No compatible Direct3D object could be created.", "Fatal Error!", MB_OK | MB_ICONSTOP | MB_APPLMODAL );
    return false;
  } // End if failure

  // Fill out a simple set of present parameters
  ZeroMemory( &m_D3DPresentParams, sizeof(D3DPRESENT_PARAMETERS) );

  // Select back buffer format etc
	m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &CurrentMode);
	
	D3DFORMAT adapterFormat = (m_bWindowed) ? CurrentMode.Format : D3DFMT_X8R8G8B8;

	m_D3DPresentParams.Windowed						= m_bWindowed;
	m_D3DPresentParams.BackBufferFormat				= CurrentMode.Format;
  m_D3DPresentParams.BackBufferWidth				= (m_bWindowed) ? 0 : CurrentMode.Width;
  m_D3DPresentParams.BackBufferHeight				= (m_bWindowed) ? 0 : CurrentMode.Height;
	m_D3DPresentParams.BackBufferCount				= 1;
  m_D3DPresentParams.SwapEffect					= D3DSWAPEFFECT_DISCARD;
  m_D3DPresentParams.hDeviceWindow				= hWnd;
  m_D3DPresentParams.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;
	m_D3DPresentParams.Windowed						= bWindowedMode;
	m_D3DPresentParams.AutoDepthStencilFormat		= FindDepthStencilFormat( D3DADAPTER_DEFAULT, CurrentMode, D3DDEVTYPE_HAL );
	m_D3DPresentParams.EnableAutoDepthStencil		= true;
	m_D3DPresentParams.FullScreen_RefreshRateInHz = (m_bWindowed) ? 0 : CurrentMode.RefreshRate;
    
	// Set Creation Flags
	unsigned long ulFlags = 0;// D3DCREATE_SOFTWARE_VERTEXPROCESSING;

  // Check if Hardware T&L is available
  ZeroMemory( &Caps, sizeof(D3DCAPS9) );
  m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps );
/*
	// If particle sizing is allowed
	if (Caps.FVFCaps & D3DFVFCAPS_PSIZE)
	{
		// If this video card will support hardware vertex processing
		if ( Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
			ulFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	if ( Caps.DevCaps & D3DDEVCAPS_PUREDEVICE )
	{
		ulFlags |= D3DCREATE_PUREDEVICE;
	}*/
  
    //DWORD dwBehaviorFlags = 0;

    if( Caps.VertexProcessingCaps != 0 )
        ulFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        ulFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

    // Attempt to create a HAL device
    if( FAILED( hRet = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, ulFlags, &m_D3DPresentParams, &m_pD3DDevice ) ) ) 
    {
        MessageBox( hWnd, "Could not create a valid HAL Direct3D device object.\r\n\r\n" 
                           "The system will now attempt to create a device utilising the 'Reference Rasterizer' (D3DDEVTYPE_REF)",
                           "Fatal Error!", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL) ;
        
        // Find REF depth buffer format
        m_D3DPresentParams.AutoDepthStencilFormat = FindDepthStencilFormat( D3DADAPTER_DEFAULT, CurrentMode, D3DDEVTYPE_REF );

        // Check if Hardware T&L is available
        ZeroMemory( &Caps, sizeof(D3DCAPS9) );
        ulFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, &Caps );
        if ( Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
			ulFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

        // Attempt to create a REF device
        if( FAILED( hRet = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, ulFlags, &m_D3DPresentParams, &m_pD3DDevice ) ) ) 
        {
            MessageBox( hWnd, "Could not create a valid REF Direct3D device object.\r\n\r\nThe system will now exit.",
                              "Fatal Error!", MB_OK | MB_ICONSTOP | MB_APPLMODAL );

            // Failed
            return false;
        
        } // End if Failure (REF)

    } // End if Failure (HAL)


	//Set vertex shader
    m_pD3DDevice->SetVertexShader(NULL);
    m_pD3DDevice->SetFVF (D3DFVF_TLVERTEX);

    //Create vertex buffer and set as stream source
    m_pD3DDevice->CreateVertexBuffer(sizeof(TLVERTEX) * 4, NULL, D3DFVF_TLVERTEX, D3DPOOL_MANAGED,
                                  &vertexBuffer, NULL);
    m_pD3DDevice->SetStreamSource (0, vertexBuffer, 0, sizeof(TLVERTEX));

    //Setup rendering states
    m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// Create the fonts
	CreateDXFonts();


  if(FAILED( D3DXCreateSprite(m_pD3DDevice, &m_pSprite)) )
  {
    g_Log.Write("Sprite creation failure...");
    return false;
  }


    // Success!!
    return true;
}


//-----------------------------------------------------------------------------
// Name : FindDepthStencilFormat ()		(private)
// Desc : This function simply determines the best depth format that is
//        available for the specified mode.
// Note : No tests for stencil active depth buffers are made.
//-----------------------------------------------------------------------------
D3DFORMAT CDXGraphics::FindDepthStencilFormat( ULONG AdapterOrdinal, D3DDISPLAYMODE Mode, D3DDEVTYPE DevType )
{

    // Test for 24 bith depth buffer
    if (SUCCEEDED( m_pD3D->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D32 )))
    {
        if (SUCCEEDED( m_pD3D->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D32 ))) return D3DFMT_D32;
    
    } // End if 32bpp Available

    // Test for 24 bit depth buffer
    if (SUCCEEDED( m_pD3D->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D24X8 )))
    {
        if (SUCCEEDED( m_pD3D->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D24X8 ))) return D3DFMT_D24X8;
    
    } // End if 24bpp Available

    // Test for 16 bit depth buffer
    if (SUCCEEDED( m_pD3D->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D16 )))
    {
        if (SUCCEEDED( m_pD3D->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D16 ))) return D3DFMT_D16;
    
    } // End if 16bpp Available

    // No depth buffer supported
    return D3DFMT_UNKNOWN;

}


void CDXGraphics::CreateDXFonts()
{
  //load 12 different fonts for Arial and Verdana, 12-16, normal and bold
    D3DXCreateFont(m_pD3DDevice, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A12] );
    D3DXCreateFont(m_pD3DDevice, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A12B] );
    D3DXCreateFont(m_pD3DDevice, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A14] );
    D3DXCreateFont(m_pD3DDevice, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A14B] );
    D3DXCreateFont(m_pD3DDevice, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A16] );
    D3DXCreateFont(m_pD3DDevice, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A16B] );
    D3DXCreateFont(m_pD3DDevice, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V12] );
    D3DXCreateFont(m_pD3DDevice, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V12B] );
    D3DXCreateFont(m_pD3DDevice, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V14] );
    D3DXCreateFont(m_pD3DDevice, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V14B] );
    D3DXCreateFont(m_pD3DDevice, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V16] );    
    D3DXCreateFont(m_pD3DDevice, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V16B] );

    D3DXCreateFont(m_pD3DDevice, 20, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V20] );
    D3DXCreateFont(m_pD3DDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V20B] );

}
void CDXGraphics::Draw2DText(std::string sText, int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;
  else if(fontType == 12 || fontType == 13)
    fontHeight = 20;

  width = sText.size() * fontHeight;
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  m_Font[fontType]->DrawText(NULL, sText.c_str(), -1, &rct, 0, fontColor);  
}

void CDXGraphics::Draw2DTextValue(std::string sText, float value,int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  std::ostringstream oss;
  std::string text;
  oss << sText << ":" << value;
  text = oss.str();
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;
  else if(fontType == 12 || fontType == 13)
    fontHeight = 20;

  width = sText.size() * fontHeight * 20;
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  m_Font[fontType]->DrawText(NULL, text.c_str(), -1, &rct, 0, fontColor);  
}


void CDXGraphics::Draw2DValue(float value,int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  std::ostringstream oss;
  std::string text;
  oss << value;
  text = oss.str();
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;
  if(fontType < 0 || fontType > 11)
    fontType = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;

  width = text.size() * fontHeight;
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  m_Font[fontType]->DrawText(NULL, text.c_str(), -1, &rct, 0, fontColor);  
}

void CDXGraphics::Draw2DValue(int value,int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  std::ostringstream oss;
  std::string text;
  oss << value;
  text = oss.str();
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;
  if(fontType < 0 || fontType > 11)
    fontType = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;

  width = text.size() * fontHeight;
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  m_Font[fontType]->DrawText(NULL, text.c_str(), -1, &rct, 0, fontColor);  
}


//Close Direct3D
int CDXGraphics::CloseD3D()
{
	// Release the textures
	for (int i = 0; i < m_TextureCount; i++)
	{
		if (m_pFile[i])
		{
			m_pFile[i]->Release();
			m_pFile[i] = NULL;
		}
	}

    for(int i = 0; i < C_NUMBER_FONTS; i++){
      if(m_Font[i])
        m_Font[i]->Release();
    }

    //Release vertex buffer
    if (vertexBuffer)
	{
        vertexBuffer->Release ();
		vertexBuffer = NULL;
	}

    //Release device
    if (m_pD3DDevice)
	{
        m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

    //Release m_pD3D object
    if (m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

    if(m_pSprite)
	{
      m_pSprite->Release();
	  m_pSprite = NULL;
	}

    //Successfully closed Direct3D
    return TRUE;
}

//draws circle
void CDXGraphics::DrawCircle(float x, float y, int red, int green, int blue, float radius){

  const int NUMPOINTS = 24;
  const float PI = 3.14159;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex circle[NUMPOINTS + 1];
  const DWORD line_fvf=D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  
  float theta;
  float wedgeAngle;
  wedgeAngle = (float)((2 * PI)/NUMPOINTS);
  for (int i = 0; i <= NUMPOINTS; i++){
    theta = i * wedgeAngle;
    circle[i].x = (float)(x + radius * cos(theta));
    circle[i].y = (float)(y - radius * sin(theta));
    circle[i].z = 0;
    circle[i].rhw = 1.0f;
    circle[i].color = D3DCOLOR_XRGB(red, green, blue);
  }

  m_pD3DDevice->SetFVF(line_fvf);
  m_pD3DDevice->SetTexture(0, NULL);
  m_pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &circle[0],sizeof(circle[0]));
  
}

//Clear backbuffer
int CDXGraphics::ClearBuffer (int red, int green, int blue, int alpha){ 
  return m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(alpha, red, green, blue), 0.0f, 0);
}

//Present
int CDXGraphics::Present () {
  return m_pD3DDevice->Present (NULL, NULL, NULL, NULL);
}
   
//Begin drawing
int CDXGraphics::BeginDrawing () { return m_pD3DDevice->BeginScene (); }

//End drawing
int CDXGraphics::EndDrawing () { return m_pD3DDevice->EndScene (); }

//*******************************************************************************
// RenderGraphic(...)  - draws defined tile or sprite
//*******************************************************************************
void CDXGraphics::RenderGraphic(int positionX, int positionY, GRAPHIC_IMAGE gi){
  if(gi.fileID > GF_MAX_FILES - 1)
    return;
  

  RECT rect;
  if(gi.frameMax == 0){//non-animated
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }
  else if(gi.frameMax >0 ){               //animated
    rect.left = gi.sourceX + (gi.width * gi.frameCount);
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }
  else if(gi.frameMax < 0 ){               //animated
    rect.left = gi.sourceX + (gi.width * abs(gi.frameCount));
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }
  else{
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }
  
  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
//  d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

  D3DXVECTOR2 pos;
  pos.x = positionX;
  pos.y = positionY;
  D3DXVECTOR2 scaling;
  scaling.x = gi.scale;
  scaling.y = gi.scale;
  D3DXVECTOR2 rotcenter;
  rotcenter.x = gi.rotationX;
  rotcenter.y = gi.rotationY;
  D3DXMATRIX mat;
  D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, gi.rotationAngle, &pos);
  m_pSprite->SetTransform(&mat);  
  
  if(FAILED( m_pSprite->Draw(m_pFile[gi.fileID], &rect, NULL, NULL, D3DCOLOR_ARGB(gi.alpha, 255, 255, 255)) ))
  {
    g_Log.Write("Sprite Draw failure!", gi.fileID);
    g_Log.Write("Pointer",m_pFile[gi.fileID]);
  }

  D3DXMatrixIdentity(&mat);
  m_pSprite->SetTransform(&mat);
  m_pSprite->End();
 
}

void CDXGraphics::RenderGraphicModulate(int positionX, int positionY, GRAPHIC_IMAGE gi, int red, int green, int blue){
  if(gi.fileID > GF_MAX_FILES - 1)
    return;
 
  RECT rect;
  if(gi.frameMax == 0){//non-animated
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }
  else if(gi.frameMax >0 ){               //animated
    rect.left = gi.sourceX + (gi.width * gi.frameCount);
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }
  else if(gi.frameMax < 0 ){               //animated
    rect.left = gi.sourceX + (gi.width * abs(gi.frameCount));
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }
  else{
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }

/*
  if(gi.frameMax == 0){//non-animated
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }
  else if(gi.frameMax >0 ){               //animated
    rect.left = gi.sourceX + (gi.width * gi.frameCount);
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }
  else{
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = rect.top + gi.height;
  }*/

  
  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
  m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

  D3DXVECTOR2 pos;
  pos.x = positionX;
  pos.y = positionY;
  D3DXVECTOR2 scaling;
  scaling.x = gi.scale;
  scaling.y = gi.scale;
  D3DXVECTOR2 rotcenter;
  rotcenter.x = gi.rotationX;
  rotcenter.y = gi.rotationY;
  D3DXMATRIX mat;
  D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, gi.rotationAngle, &pos);
  m_pSprite->SetTransform(&mat);  
  if(FAILED(m_pSprite->Draw(m_pFile[gi.fileID], &rect, NULL, NULL, D3DCOLOR_ARGB(gi.alpha, red, green, blue)) )){
   // g_Log.Write("Sprite Draw failure!", gi.fileID);
  }

  D3DXMatrixIdentity(&mat);
  m_pSprite->SetTransform(&mat);
  m_pSprite->End();

}


//*****************************************************************
// GetTexturePointer(...)
//*****************************************************************
IDirect3DTexture9 * CDXGraphics::GetTexturePointer(int fileID){
  return m_pFile[fileID];
}

//*****************************************************************
// LoadTexture(...)
//*****************************************************************
void CDXGraphics::LoadTexture(std::string fileName, int fileID)
{
    IDirect3DTexture9 *d3dTexture;
    D3DXIMAGE_INFO SrcInfo;			//Optional

    //Use a magenta colorkey
    D3DCOLOR colorkey = 0xFFFF00FF;

    // Load image from file
	if (FAILED(D3DXCreateTextureFromFileEx (m_pD3DDevice, fileName.c_str(), 0, 0, 1, 0, 
          D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
          colorkey, &SrcInfo, NULL, &d3dTexture)))
    {
      g_Log.Write("CreateTexture failed.......");  
      return;// NULL;
    }
   
	//Return the newly made texture
  m_pFile[fileID] = d3dTexture;
	m_TextureCount++;

  g_Log.Write("Texture number loaded", fileID);
}


RECT CDXGraphics::LoadRect(float left, float top, float right, float bottom){
  RECT temp;
  temp.left = (long)left;
  temp.top = (long)top;
  temp.right = (long)right;
  temp.bottom = (long)bottom;
  return temp; 
}

int CDXGraphics::GetScreenCenterX(){
  return m_screenCenterX;
}
int CDXGraphics::GetScreenCenterY(){
  return m_screenCenterY;
}
int CDXGraphics::GetScreenWidth(){
  //m_screenWidth = ::GetSystemMetrics(SM_CYSCREEN);
  return m_screenWidth;
}
int CDXGraphics::GetScreenHeight(){
  //m_screenHeight = ::GetSystemMetrics(SM_CXSCREEN);                               
  return m_screenHeight;
}

void CDXGraphics::SetScreenWidthHeight(int w, int h){
 m_screenWidth = w;
 m_screenHeight = h;
 m_screenCenterX = m_screenWidth/2;
 m_screenCenterY = m_screenHeight/2;
}

//captures screen and saves to file
HRESULT CDXGraphics::ScreenGrab(char *fileName){
  HRESULT hr;

  ::D3DDISPLAYMODE mode;
  if(FAILED(hr=m_pD3DDevice->GetDisplayMode(0, &mode)))
    return 100;

  ::LPDIRECT3DSURFACE9 surf;
  if(FAILED(hr=m_pD3DDevice->CreateOffscreenPlainSurface(mode.Width, mode.Height,D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surf, NULL)))
    return 200;

  if(FAILED(hr=m_pD3DDevice->GetFrontBufferData(0, surf))){
    surf->Release();
    return 300;
  }

  hr = ::D3DXSaveSurfaceToFile(fileName, D3DXIFF_JPG, surf, NULL, NULL);
  surf->Release();
  return 0;
}