<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{

			$this->GetWebsite()->IncludeModule("fractal/fractal");

			$arrayConfig = array();
			$arrayConfig['height'] = 620;
			$arrayConfig['width'] = 1140;
			$fractal = new CFractalModule($this->GetWebsite(),$arrayConfig);
			$fractal->SetState('tree');
			$arrayItems["MODULE_FRACTAL"] = $fractal->RenderModule();
		
		}
	}
	
