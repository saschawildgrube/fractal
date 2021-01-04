<?php

	class CFractalModule extends CModule
	{
	
		function __construct(
			$website,
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				'fractal/fractal',
				'',
				$arrayConfig);
		}
		
		function OnInit()
		{
			if ($this->GetState() == '')
			{				
				$this->SetState('mandelbrot');	
			}
			return parent::OnInit();
		}

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case 'tree':
				{
					return $this->StateTree();
				}
				case 'mandelbrot':
				default:
				{
					return $this->StateMandelbrot();
				}
			}
		}
		
		
		function StateMandelbrot()
		{
			$this->SetState('mandelbrot');
			
			$nHeight = intval($this->GetModuleConfig('height'));
			if ($nHeight == 0)
			{
				$nHeight = 600;
			}
			$nWidth = intval($this->GetModuleConfig('width'));
			if ($nWidth == 0)
			{
				$nWidth = 600;
			}
			
			$fRatio = $nWidth / $nHeight;
			
			$arrayImageParams = array();
			$arrayImageParams['command'] = 'image';
			$arrayImageParams['id'] = 'fractal_mandelbrot';
			$arrayImageParams['width'] = $nWidth;
			$arrayImageParams['height'] = $nHeight;    
			$arrayImageParams['x'] = 0.53;
			$arrayImageParams['y'] = 0; 
			$arrayImageParams['horizontal'] = 2.9 + (($fRatio>1)?($fRatio):(-$fRatio));

			
			$this->GetWebsite()->Trace($arrayImageParams);
			
			$strImageURL = $this->GetWebsite()->MakeURL($arrayImageParams,false);
			
			$this->IncludeElement('image');
			$image = new CImageElement($this->GetWebsite(),'default');
			$this->SetOutputItem(
				'IMAGE_MANDELBROT',
				$image->RenderImage(
					$strImageURL,
					'') 
					);
					 
			return 'mandelbrot';
		}
		
		function StateTree()
		{
			$this->SetState('tree');


			$nHeight = intval($this->GetModuleConfig('height'));
			if ($nHeight == 0)
			{
				$nHeight = 600;
			}
			$nWidth = intval($this->GetModuleConfig('width'));
			if ($nWidth == 0)
			{
				$nWidth = 600;
			}
			
			$nSeed = time();
			
			$arrayImageParams = array();
			$arrayImageParams["command"] = 'image';
			$arrayImageParams["id"] = 'fractal_tree';
			$arrayImageParams['width'] = $nWidth;
			$arrayImageParams['height'] = $nHeight;    
			$arrayImageParams["rootlength"] = $arrayImageParams["height"] / 5;
			$arrayImageParams["nextbranch1"] = 0.5 	+ (sin(deg2rad(1*fmod($nSeed,360)))*0.3);
			$arrayImageParams["nextbranch2"] = 0.5 	+ (sin(deg2rad(2*fmod($nSeed,360)))*0.3);
			$arrayImageParams["angle1"] = -30 	 		+ (sin(deg2rad(3*fmod($nSeed,360)))*30);
			$arrayImageParams["angle2"] = 30 			+ (sin(deg2rad(4*fmod($nSeed,360)))*30);
			
			$this->GetWebsite()->Trace("nSeed = $nSeed");
			$this->GetWebsite()->Trace($arrayImageParams);
			
			
			$strImageURL = $this->GetWebsite()->MakeURL($arrayImageParams,false);
			
			$this->IncludeElement("image");
			$image = new CImageElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				'IMAGE_TREE',
				$image->RenderImage(
					$strImageURL,
					'')
					);
					 
			$this->GetWebsite()->Redirect($this->GetWebsite()->MakeSelfURL(false),1);
	
			return 'tree';
		}		

	}

