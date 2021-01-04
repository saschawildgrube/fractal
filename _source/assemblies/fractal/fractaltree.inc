<?php

	function CompassDegreesToMathDegrees($fAngle)
	{
		//return fmod(fmod(360-$fAngle,360)-90+360,360);
		return (90+360+(-(fmod($fAngle,360)+360)));
	}
	
	
	function DrawRecursiveFractalTree(
		$image,
		$nRootX,
		$nRootY,
		$nRootLength,
		$nRootAngle,
		$nAngle1,
		$nAngle2,
		$fNextBranch1,
		$fNextBranch2,
		$colorTree,
		$nRecursions)
	{
		
		$fRootAngle = deg2rad(CompassDegreesToMathDegrees($nRootAngle));
		
		$nEndX = $nRootX + (cos($fRootAngle)*$nRootLength);
		
		$nEndY = $nRootY - (sin($fRootAngle)*$nRootLength);
		//$nEndY = $nRootY - $nRootLength;
		
		imageline ($image , $nRootX , $nRootY , $nEndX , $nEndY, $colorTree );
		if ($nRecursions > 0)
		{
			// Branch 1
			DrawRecursiveFractalTree(
				$image,
				$nEndX,
				$nEndY,
				intval($nRootLength * floatval($fNextBranch1)),
				$nRootAngle+$nAngle1,
				$nAngle1,
				$nAngle2,
				$fNextBranch1,
				$fNextBranch2,
				$colorTree,
				$nRecursions-1);
	
			// Branch 2
			DrawRecursiveFractalTree(
				$image,
				$nEndX,
				$nEndY,
				intval($nRootLength * floatval($fNextBranch2)),
				$nRootAngle+$nAngle2,
				$nAngle1,
				$nAngle2,
				$fNextBranch1,
				$fNextBranch2,
				$colorTree,
				$nRecursions-1);
		}
		return true;
	}
	
	function CreateFractalTreeImage(
		&$image,
		$nWidth,
		$nHeight,
		$nRootLength,
		$nAngle1,
		$nAngle2,
		$fNextBranch1,
		$fNextBranch2,
		$nRecursions)
	{
		$nWidth = intval(($nWidth)?($nWidth):(100));
		$nHeight = intval(($nHeight)?($nHeight):(100));
		
		$fNextBranch1 = floatval($fNextBranch1);
		$fNextBranch2 = floatval($fNextBranch2);
		
		$nRootLength = intval($nRootLength);
		$nRootLength = ($nRootLength)?($nRootLength):(intval($nHeight/3));
		
		
		$image = imagecreate($nWidth,$nHeight);
		if ($image == 0)
		{
			return false;	
		}
		
		//$colorBackground =  imagecolorallocate ( $image, 255, 255, 255);
		//$colorTree =  imagecolorallocate ( $image, 0, 0, 0);

		$colorBackground = imagecolorallocate ( $image, 0, 0, 0);
		$colorTree =  imagecolorallocate ( $image, 255, 255, 255);

		
		imagefilledrectangle($image, 0, 0, $nWidth, $nHeight, $colorBackground);
		
		return DrawRecursiveFractalTree(
			$image,
			$nWidth / 2,
			$nHeight,
			$nRootLength,
			0, // North
			$nAngle1,
			$nAngle2,
			$fNextBranch1,
			$fNextBranch2,
			$colorTree,
			$nRecursions);
	}

