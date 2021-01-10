<?php

	require_once(GetAssemblyDir('fractal').'fractaltree.inc');
	require_once(GetAssemblyDir('fractal').'mandelbrot.inc');

	class CFractalPlugin extends CWebsitePlugin
	{
		function __construct(
			$website)
		{
			parent::__construct(
				$website,
				'fractal');
		}
		
		function OnCreateImage(&$image,$strImageID)
		{
			if ($strImageID == 'fractal_tree')	
			{
				$bResult = CreateFractalTreeImage(
					$image,
					$this->GetParam('width'),
					$this->GetParam('height'),
					$this->GetParam('rootlength'),
					$this->GetParam('angle1'),
					$this->GetParam('angle2'),
					$this->GetParam('nextbranch1'),
					$this->GetParam('nextbranch2'),
					10);
				return $bResult;
			}
			if ($strImageID == 'fractal_mandelbrot')	
			{
				$bResult = CreateFractalMandelbrotImage(
					$image,
					$this->GetParam('width'),
					$this->GetParam('height'),
					$this->GetParam('x'),
					$this->GetParam('y'),
					$this->GetParam('horizontal')
					);
				return $bResult;
			}			
			return parent::OnCreateImage($image,$strImageID);
		}
		
		
		
	}
