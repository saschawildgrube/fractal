<?php



/*
     function createcolor($pic,$c1,$c2,$c3) {
          //get color from palette
          $color = imagecolorexact($pic, $c1, $c2, $c3);
          if($color==-1) {
               //color does not exist...
               //test if we have used up palette
               if(imagecolorstotal($pic)>=255) {
                    //palette used up; pick closest assigned color
                    $color = imagecolorclosest($pic, $c1, $c2, $c3);
               } else {
                    //palette NOT used up; assign new color
                    $color = imagecolorallocate($pic, $c1, $c2, $c3);
               }
          }
          return $color;
     }
     
*/
  


	function CalculateMandelbrotCoordinate($fX, $fY, $nColors, $nMaxIterations)
	{
		$nIteration = 0;
		$bDivergence = false;
		$fzR = 0;
		$fzI = 0;
		$feMax = 2;
		$feMaxSqr = $feMax * $feMax;
		while ($nIteration < $nMaxIterations && $bDivergence == false)
		{
			$fzR_new = ($fzR * $fzR) - ($fzI * $fzI) - $fX;
			$fzI_new = 2 * $fzR * $fzI - $fY;

			$fDistanceSqr = ($fzR_new * $fzR_new) + ($fzI_new * $fzI_new); 
			if ($fDistanceSqr > $feMaxSqr)
			{
				$bDivergence = true;
			}
			else
			{
				$fzR = $fzR_new;
				$fzI = $fzI_new;
				$nIteration++;
			}
		}
		
		$nColor = 0; 
		if ($bDivergence == true) 
		{
			$nColor = ($nIteration % ($nColors-1)) + 1;
		}
		return $nColor;		
	}

	function CalculateMandelbrotMapRect(&$map, $nLeft, $nTop, $nWidth, $nHeight, $fTopLeftX, $fTopLeftY, $fPixelSize, $nColors, $nMaxIterations)
	{
		$bRecursive = false;
		$bBoundingRect = false;

		// The rectangle must be big enough so that recursion pays off
		if ($nWidth > 20)
		{
			if ($nHeight > 20) 
			{
				$bRecursive = true;
			}
		}
		
		if ($fTopLeftX < -2 && $fTopLeftY > 2)  
		{
			$fBottomRightX = $fTopLeftX + ($nWidth * $fPixelSize);
			if ($fBottomRightX > 2)
			{
				$fBottomRightY = $fTopLeftY - ($nHeight * $fPixelSize);
				if ($fBottomRightY < -2)
				{
					$bBoundingRect = true;
				}
					
			}
		}

		// We only analyse the borders for a constant color if the rect is not fully overlapping the bounding rect of the outer circle
		if ($bBoundingRect == false)
		{
			$bSameColor = true;
			$nFirstColor = null;
			
			// Top row
			$fY = $fTopLeftY;
			$fX = $fTopLeftX;
			for ($nIndexX	= $nLeft; $nIndexX < $nWidth + $nLeft; $nIndexX++)
			{
				$nColor = CalculateMandelbrotCoordinate($fX, $fY, $nColors, $nMaxIterations);
				if ($nFirstColor === null)
				{
					$nFirstColor = $nColor;
				}
				else
				{
					if ($nFirstColor != $nColor)
					{
						$bSameColor = false;
					}
				}
				$map[$nTop][$nIndexX] = $nColor;
				$fX += $fPixelSize;
			}
			
		
			// Bottom row
			$fY = $fTopLeftY - (($nHeight-1) * $fPixelSize) ;
			$fX = $fTopLeftX;
			for ($nIndexX	= $nLeft; $nIndexX < $nWidth + $nLeft; $nIndexX++)
			{
				$nColor = CalculateMandelbrotCoordinate($fX, $fY, $nColors, $nMaxIterations);
				if ($nFirstColor != $nColor)
				{
					$bSameColor = false;
				}
				$map[$nTop + $nHeight - 1][$nIndexX] = $nColor;
				$fX += $fPixelSize;
			}

		
			// Left Column
			$fY = $fTopLeftY - $fPixelSize;
			$fX = $fTopLeftX;
			for ($nIndexY	= $nTop + 1; $nIndexY < $nTop + 1 + $nHeight - 2; $nIndexY++)
			{
				$nColor = CalculateMandelbrotCoordinate($fX, $fY, $nColors, $nMaxIterations);
				if ($nFirstColor != $nColor)
				{
					$bSameColor = false;
				}
				$map[$nIndexY][$nLeft] = $nColor;
				$fY -= $fPixelSize;
			}

		
			// Right Column
			$fY = $fTopLeftY - $fPixelSize;
			$fX = $fTopLeftX + (($nWidth-1) * $fPixelSize);
			for ($nIndexY	= $nTop + 1; $nIndexY < $nTop + 1 + $nHeight - 2; $nIndexY++)
			{
				$nColor = CalculateMandelbrotCoordinate($fX, $fY, $nColors, $nMaxIterations);
				if ($nFirstColor != $nColor)
				{
					$bSameColor = false;
				}
				$map[$nIndexY][$nLeft + $nWidth - 1] = $nColor;
				$fY -= $fPixelSize;
			}

			
			
			if ($bSameColor == true)
			{
				// set all pixels within the rect to the same color
				for ($nIndexY = $nTop + 1; $nIndexY < $nTop + 1 + $nHeight - 2; $nIndexY++) 
				{
					for ($nIndexX	= $nLeft + 1; $nIndexX < $nLeft + 1 + $nWidth - 2; $nIndexX++)
					{
						$map[$nIndexY][$nIndexX] = $nFirstColor;
					}
				}
				return;
			}
			
			$nLeft++;
			$nTop++;
			$nWidth-=2;
			$nHeight-=2;
			$fTopLeftX += $fPixelSize;
			$fTopLeftY -= $fPixelSize;
		}
		
		// Recursive rects

		if ($bRecursive == true)
		{
			$nWidthLeft = intval(round($nWidth / 2));
			$nWidthRight = $nWidth - $nWidthLeft;
			$nHeightTop = intval(round($nHeight / 2));
			$nHeightBottom = $nHeight - $nHeightTop;

			// top left
			CalculateMandelbrotMapRect(
				$map,
				$nLeft, $nTop,
				$nWidthLeft, $nHeightTop,
				$fTopLeftX, $fTopLeftY,
				$fPixelSize,
				$nColors,
				$nMaxIterations);

			// top right
			CalculateMandelbrotMapRect(
				$map,
				$nLeft + $nWidthLeft, $nTop,
				$nWidthRight, $nHeightTop,
				$fTopLeftX + (($nWidthLeft - 1) * $fPixelSize), $fTopLeftY,
				$fPixelSize,
				$nColors,
				$nMaxIterations);

			// bottom left
			CalculateMandelbrotMapRect(
				$map,
				$nLeft, $nTop + $nHeightTop,
				$nWidthLeft, $nHeightBottom,
				$fTopLeftX, $fTopLeftY - (($nHeightTop-1) * $fPixelSize),
				$fPixelSize,
				$nColors,
				$nMaxIterations);

			// bottom right
			CalculateMandelbrotMapRect(
				$map,
				$nLeft + $nWidthLeft, $nTop + $nHeightTop,
				$nWidthRight, $nHeightBottom,
				$fTopLeftX + (($nWidthLeft - 1) * $fPixelSize), $fTopLeftY - (($nHeightTop-1) * $fPixelSize),
				$fPixelSize,
				$nColors,
				$nMaxIterations);

			return;
		}


		// Fallback to line by line, pixel by pixel calculation

		$fY = $fTopLeftY;
		for ($nIndexY = $nTop; $nIndexY < $nHeight + $nTop; $nIndexY++) 
		{
			$fX = $fTopLeftX;
			for ($nIndexX	= $nLeft; $nIndexX < $nWidth + $nLeft; $nIndexX++)
			{
				$map[$nIndexY][$nIndexX] = CalculateMandelbrotCoordinate($fX, $fY, $nColors, $nMaxIterations);
				$fX += $fPixelSize;
			}
			$fY -= $fPixelSize;
		}	
		
	}

 
	function CalculateMandelbrotMap($nWidth, $nHeight, $fCenterX, $fCenterY, $fHorizontal, $nColors, $nMaxIterations = 300)
	{
		$map = array();
		
		$fPixelSize = $fHorizontal / $nWidth;
		$fTopLeftX = $fCenterX - (($nWidth / 2) * $fPixelSize);
		$fTopLeftY = $fCenterY + (($nHeight / 2) * $fPixelSize);
		$fX = $fTopLeftX;
		$fY = $fTopLeftY;

		$nMaxIterations = intval($nMaxIterations);
		
/* 
		for ($nIndexY = 0; $nIndexY < $nHeight; $nIndexY++) 
		{
			$fX = $fTopLeftX;
			for ($nIndexX	= 0; $nIndexX < $nWidth ; $nIndexX++)
			{
				$map[$nIndexY][$nIndexX] = CalculateMandelbrotCoordinate($fX, $fY, $nColors, $nMaxIterations);
				$fX += $fPixelSize;
			}
			$fY -= $fPixelSize;
		
		}
*/
	
		CalculateMandelbrotMapRect(
			$map,
			0, 0,
			$nWidth, $nHeight,
			$fTopLeftX, $fTopLeftY,
			$fPixelSize,
			$nColors,
			$nMaxIterations);


		return $map;
	}
	
	function CreateFractalMandelbrotImage(
		&$image,
		$nWidth,
		$nHeight,
		$fCenterX,
		$fCenterY,
		$fHorizontal)
	{
		
	
		$nWidth = min(intval(($nWidth)?($nWidth):(100)),2000);
		$nHeight = min(intval(($nHeight)?($nHeight):(100)),2000);
		
		$fCenterX = floatval($fCenterX);
		$fCenterY = floatval($fCenterY);
		$fHorizontal = floatval($fHorizontal);

		
		$image = imagecreate($nWidth,$nHeight);
		if ($image == 0)
		{
			return false;	
		}
		
		$colorBackground = imagecolorallocate($image,0,0,0);
		imagefilledrectangle($image,0,0,$nWidth,$nHeight,$colorBackground);
	
		// Refactor: Make Color Palette (from color to color, via color)
	
		$arrayColors = array();
		$nColors = 60;
		for ($nIndexColor = 0; $nIndexColor < $nColors; $nIndexColor++)
		{
			if ($nIndexColor < $nColors/2)
			{
				$colorNew = imagecolorallocate(
					$image,
					0,
					0,
					intval((256 / ($nColors/2)) * $nIndexColor));
			}
			else
			{
				$colorNew = imagecolorallocate($image,
					min(255,intval((256 / ($nColors/2)) * ($nIndexColor-($nColors/2-1)))),
					min(255,intval((256 / ($nColors/2)) * ($nIndexColor-($nColors/2-1)))),
					255);
			}
			
			if ($colorNew !== false)
			{
				$arrayColors[] = $colorNew;
			}
		}
		
		if (ArrayCount($arrayColors) != $nColors)
		{
			return false;	
		}
		
		
		$map = CalculateMandelbrotMap($nWidth, $nHeight, $fCenterX, $fCenterY, $fHorizontal, $nColors);
		if ($map == false)
		{
			return false;	
		}
		
		
		for ($nIndexY = 0; $nIndexY < $nHeight; $nIndexY++)
		{
			for ($nIndexX	= 0; $nIndexX < $nWidth ; $nIndexX++)
			{
				
				$colorPixel = $arrayColors[$map[$nIndexY][$nIndexX]]; 
				imagesetpixel($image,$nIndexX, $nIndexY, $colorPixel);
			}
		
		}

		return true;
	}

