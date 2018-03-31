#ifndef TEECHARTDEFINES_H
#define TEECHARTDEFINES_H

// EAnnotationPosition
const unsigned long ppLeftTop = 0; 
const unsigned long ppLeftBottom = 1;
const unsigned long ppRightTop = 2; 
const unsigned long ppRightBottom = 3;
const unsigned long ppCenter = 4;

// EArrowHeadStyle
const unsigned long ahNone = 0;
const unsigned long ahLine = 1;
const unsigned long ahSolid = 2;

// EAxisArrowToolPosition
const unsigned long aaStart = 0; 
const unsigned long aaEnd = 1; 
const unsigned long aaBoth = 2;

// EAxisLabelAlign
const unsigned long alDefault = 0; 
const unsigned long alOpposite = 1;

// EAxisLabelStyle
const unsigned long talAuto = 0;
const unsigned long talNone = 1;
const unsigned long talValue = 2;
const unsigned long talMark = 3;
const unsigned long talText = 4;

// EAxisType
const unsigned long atLeft = 0;
const unsigned long atTop = 1;
const unsigned long atRight = 2;
const unsigned long atBottom = 3;
const unsigned long atDepth = 4;
const unsigned long atCustom = 5;

// EBackImageMode 
const unsigned long pbmStretch = 0;
const unsigned long pbmTile = 1;
const unsigned long pbmCenter = 2;
const unsigned long pbmCustom = 3;

// EBarStyle
const unsigned long bsRectangle = 0;
const unsigned long bsPyramid = 1;
const unsigned long bsInvPyramid = 2;
const unsigned long bsCilinder = 3;
const unsigned long bsEllipse = 4;
const unsigned long bsArrow = 5;
const unsigned long bsRectGradient = 6;
const unsigned long bsCone = 7;
const unsigned long bsBevel = 8;
const unsigned long bsSlantCube = 9;
const unsigned long bsDiamond = 10;
const unsigned long bsInvArrow = 11;
const unsigned long bsInvCone = 12;

// EBezierStyle
const unsigned long bsWindows = 0;
const unsigned long bsBezier3 = 1;
const unsigned long bsBezier4 = 2;

// EBorderStyle
const unsigned long bsNone = 0;
const unsigned long bsSingle = 1;

// EBrushStyle
const unsigned long bsSolid = 0;
const unsigned long bsClear = 1;
const unsigned long bsHorizontal = 2;
const unsigned long bsVertical = 3;
const unsigned long bsFDiagonal = 4;
const unsigned long bsBDiagonal = 5;
const unsigned long bsCross = 6;
const unsigned long bsDiagCross = 7;
const unsigned long bsFill80 = 8;
const unsigned long bsFill60 = 9; 
const unsigned long bsFill40 = 10;
const unsigned long bsFill20 = 11;
const unsigned long bsFill10 = 12;
const unsigned long bsZigZag = 13;
const unsigned long bsVertSmall = 14; 
const unsigned long bsHorizSmall = 15;
const unsigned long bsDiagSmall = 16;
const unsigned long bsBackDiagSmall = 17;
const unsigned long bsCrossSmall = 18;
const unsigned long bsBackCrossSmall = 19;

// ECandleColorStyle
const unsigned long cssRelativeToOpen = 0;
const unsigned long cssRelativeToClose = 1;

// ECandleStyle 
const unsigned long csCandleStick = 0;
const unsigned long csCandleBar = 1;
const unsigned long csOpenClose = 2;
const unsigned long csLine = 3;

// ECanvas3DPlane
const unsigned long cpX = 0;
const unsigned long cpY = 1;
const unsigned long cpZ = 2;

// ECanvasBackMode
const unsigned long cbmNone = 0;
const unsigned long cbmTransparent = 1;
const unsigned long cbmOpaque = 2;

// ECanvasTextAlign
const unsigned long ctaLeft = 0;
const unsigned long ctaCenter = 1;
const unsigned long ctaRight = 2;

// EChartGridShow
const unsigned long cgsAuto = 0;
const unsigned long cgsNo = 1;
const unsigned long cgsYes = 2;

// EChartObjectShapeStyle
const unsigned long fosRectangle = 0;
const unsigned long fosRoundRectangle = 1;

// EChartPenStyle
const unsigned long psSolid = 0;
const unsigned long psDash = 1;
const unsigned long psDot = 2;
const unsigned long psDashDot = 3;
const unsigned long psDashDotDot = 4;
const unsigned long psClear = 5;
const unsigned long psInsideFrame = 6;
const unsigned long psSmallDots = 7;

// EChartScroll
const unsigned long pmNone = 0;	
const unsigned long pmHorizontal = 1;
const unsigned long pmVertical = 2;
const unsigned long pmBoth = 3;

// EChartTheme
const unsigned long ctDefault = 0;
const unsigned long ctExcel = 1;
const unsigned long ctClassic = 2;
const unsigned long ctBusiness = 3;
const unsigned long ctWeb = 4;
const unsigned long ctWindowsXP = 5;
const unsigned long ctBlueSky = 6;
const unsigned long ctFacts = 7;

// ECheckBoxesStyle
const unsigned long cbsCheck = 0;
const unsigned long cbsRadio = 1;

// EColorLineStyle
const unsigned long clCustom = 0; 
const unsigned long clMaximum = 1;
const unsigned long clCenter = 2; 
const unsigned long clMinimum = 3;

// EColorPalette
const unsigned long cpTeeChart = 0;
const unsigned long cpExcel = 1;
const unsigned long cpVictorian = 2;
const unsigned long cpPastel = 3;
const unsigned long cpSolid = 4;
const unsigned long cpClassic = 5;
const unsigned long cpWeb = 6;
const unsigned long cpModern = 7;
const unsigned long cpRainbow = 8;
const unsigned long cpWindowsXP = 9;
const unsigned long cpDefault = 10;
const unsigned long cpMacOS = 11;
const unsigned long cpWinVista = 12;

// ECompressionPeriod
const unsigned long ocDay = 0;
const unsigned long ocWeek = 1;
const unsigned long ocMonth = 2;
const unsigned long ocBiMonth = 3;
const unsigned long ocQuarter = 4;
const unsigned long ocYear = 5;

// EConstants
const unsigned long clTeeColor = 536870912;
const unsigned long clNone = 536870911;

// ECursorToolStyle
const unsigned long cssHorizontal = 0;
const unsigned long cssVertical = 1;
const unsigned long cssBoth = 2;
const unsigned long cssScope = 3;

// ECustomSeriesStack
const unsigned long cssNone = 0; 
const unsigned long cssOverlap = 1; 
const unsigned long cssStack = 2; 
const unsigned long cssStack100 = 3;

// EDatasourceType
const unsigned long dstAllRecords = 0; 
const unsigned long dstSingleRecord = 1;

// EDateTimeStep
const unsigned long dtOneMicrosecond = 0;
const unsigned long dtOneMillisecond = 1;
const unsigned long dtOneSecond = 2;
const unsigned long dtFiveSeconds = 3;
const unsigned long dtTenSeconds = 4;
const unsigned long dtFifteenSeconds = 5;
const unsigned long dtThirtySeconds = 6;
const unsigned long dtOneMinute = 7;
const unsigned long dtFiveMinutes = 8;
const unsigned long dtTenMinutes = 9;
const unsigned long dtFifteenMinutes = 10;
const unsigned long dtThirtyMinutes = 11;
const unsigned long dtOneHour = 12;
const unsigned long dtTwoHours =13;
const unsigned long dtSixHours = 14;
const unsigned long dtTwelveHours = 15;
const unsigned long dtOneDay = 16;
const unsigned long dtTwoDays = 17;
const unsigned long dtThreeDays = 18;
const unsigned long dtOneWeek = 19;
const unsigned long dtHalfMonth = 20;
const unsigned long dtOneMonth = 21;
const unsigned long dtTwoMonths = 22;
const unsigned long dtThreeMonths =23; 
const unsigned long dtFourMonths = 24;
const unsigned long dtSixMonths = 25;
const unsigned long dtOneYear = 26;
const unsigned long dtNone = 27;

// EDeltaImageStyle
const unsigned long disSmiles = 0;
const unsigned long disHands = 1;

// EDownSampleMethod
const unsigned long dsDouglas = 0;
const unsigned long dsMax = 1;
const unsigned long dsMin = 2;
const unsigned long dsMinMax = 3;
const unsigned long dsAverage = 4;
const unsigned long dsMinMaxFirstLast = 5;
const unsigned long dsMinMaxFirstLastNull = 5;

// EDragMode
const unsigned long dmManual = 0;
const unsigned long dmAutomatic = 1;

// EDragPointStyle
const unsigned long dsX = 0;
const unsigned long dsY = 1;
const unsigned long dsBoth = 2;

// EDragState 
const unsigned long dsDragEnter = 0;
const unsigned long dsDragLeave = 1;
const unsigned long dsDragMove = 2;

// EDrawLineStyle
const unsigned long dlLine = 0;
const unsigned long dlHorizParallel = 1;
const unsigned long dlVertParallel = 2;
const unsigned long dlRectangle = 3;
const unsigned long dlEllipse = 4;

// EErrorSeriesStyle
const unsigned long essLeft = 0;
const unsigned long essRight = 1;
const unsigned long essLeftRight = 2;
const unsigned long essTop = 3;
const unsigned long essBottom = 4;
const unsigned long essTopBottom = 5;

// EErrorWidthUnits 
const unsigned long ewuPercent = 0;
const unsigned long ewuPixels= 1;

// EFaderStyle
const unsigned long fsFadeIn = 0;
const unsigned long fsFadeOut = 1;

// EFilterClass
const unsigned long fcConvolve = 0;
const unsigned long fcBlur = 1;
const unsigned long fcResize = 2;
const unsigned long fcCrop = 3;
const unsigned long fcInvert = 4;
const unsigned long fcGrayScale = 5;
const unsigned long fcFlip = 6;
const unsigned long fcReverse = 7;
const unsigned long fcAmount = 8;
const unsigned long fcMosaic = 9;
const unsigned long fcBrightness = 10;
const unsigned long fcContrast = 11;
const unsigned long fcColor = 12;
const unsigned long fcHueLumSat = 13;
const unsigned long fcSharpen = 14;
const unsigned long fcEmboss = 15;
const unsigned long fcSoften = 16;
const unsigned long fcGammaCorrection = 17;
const unsigned long fcRotate = 18;
const unsigned long fcMirror = 19;
const unsigned long fcTile = 20;
const unsigned long fcBevel = 21;
const unsigned long fcZoom = 22;
const unsigned long fcLighting = 23;


// EFunctionPeriodAlign
const unsigned long paFirst = 0;
const unsigned long paCenter = 1;
const unsigned long paLast = 2;

// EFunctionPeriodStyle
const unsigned long psNumPoints = 0;
const unsigned long psRange = 1;

// EFunctionType
const unsigned long tfCopy = 0;
const unsigned long tfAdd = 1;
const unsigned long tfSubtract = 2;
const unsigned long tfMultiply = 3;
const unsigned long tfDivide = 4;
const unsigned long tfAverage = 5;
const unsigned long tfHigh = 6;
const unsigned long tfLow = 7;
const unsigned long tfMovavg = 8;
const unsigned long tfExpavg = 9;
const unsigned long tfMomentum = 10;
const unsigned long tfCurveFit = 11;
const unsigned long tfRSI = 12;
const unsigned long tfTrend = 13;
const unsigned long tfStdDeviation = 14;
const unsigned long tfCumulative = 15;
const unsigned long tfCount = 16;
const unsigned long tfExpTrend = 17;
const unsigned long tfExpMovAvg = 18;
const unsigned long tfMomentumDiv = 19;
const unsigned long tfRMS = 20;
const unsigned long tfMACD = 21;
const unsigned long tfStochastic = 22;
const unsigned long tfBollinger = 23;
const unsigned long tfADX = 24;
const unsigned long tfSmoothing = 25;
const unsigned long tfCustom = 26;
const unsigned long tfCompress = 27;
const unsigned long tfCrossPoints = 28;
const unsigned long tfCLV = 29;
const unsigned long tfOBV = 30;
const unsigned long tfCCI = 31;
const unsigned long tfPVO = 32;
const unsigned long tfPerformance = 33;
const unsigned long tfMode = 34;
const unsigned long tfMedian = 35;
const unsigned long tfCorrelation = 36;
const unsigned long tfVariance = 37;
const unsigned long tfPerimeter = 38;
const unsigned long tfDownSampleFun = 39;
const unsigned long tfSubset = 40;
const unsigned long tfHistogram = 41;
const unsigned long tfSAR = 42;

// EGanttToolPartBar
const unsigned long pbStart = 0;
const unsigned long pbAll = 1;
const unsigned long pbEnd = 2;

// EGIFColorReduction
const unsigned long rmNone = 0; 
const unsigned long rmWindows20 = 1; 
const unsigned long rmWindows256 = 2; 
const unsigned long rmWindowsGray = 3; 
const unsigned long rmMonochrome = 4; 
const unsigned long rmGrayScale = 5; 
const unsigned long rmNetscape = 6; 
const unsigned long rmQuantize = 7; 
const unsigned long rmQuantize256 = 8;

// EGIFCompression
const unsigned long gcLZW = 0; 
const unsigned long gcRLE = 1;

// EGIFDitherMode
const unsigned long dmNearest = 0; 
const unsigned long dmFloydSteinberg = 1; 
const unsigned long dmStucki = 2; 
const unsigned long dmSierra = 3; 
const unsigned long dmJaJuNI = 4; 
const unsigned long dmSteveArche = 5; 
const unsigned long dmBurkes = 6;

// EGradientDirection 
const unsigned long gdTopBottom = 0;
const unsigned long gdBottomTop = 1;
const unsigned long gdLeftRight = 2;
const unsigned long gdRightLeft = 3;
const unsigned long gdFromCenter = 4;
const unsigned long gdFromTopLeft = 5;
const unsigned long gdFromBottomLeft = 6;
const unsigned long gdRadial = 7;
const unsigned long gdDiagonalUp = 8;
const unsigned long gdDiagonalDown = 9;

// EGrayMethod
const unsigned long gmSimple = 0;
const unsigned long gmEye = 1;
const unsigned long gmEye2 = 2;

// EGroupFormula
const unsigned long gfCount = 0;
const unsigned long gfSum = 1;

// EHandStyle
const unsigned long hsLine = 0;
const unsigned long hsTriangle = 1;

// EHorizontalAxis
const unsigned long aTopAxis = 0;
const unsigned long aBottomAxis = 1;
const unsigned long aBothHorizAxis = 2;
const unsigned long aCustomHorizAxis = 3;

// EJPEGPerformance
const unsigned long jpegBestQuality = 0;
const unsigned long jpegBestSpeed = 1;

// ELegendAlignment
const unsigned long laLeft = 0;
const unsigned long laRight = 1;
const unsigned long laTop = 2;
const unsigned long laBottom = 3;

// ELegendPaletteAxis
const unsigned long laDefault = 0;
const unsigned long laOther = 1;
const unsigned long laBoth = 2;

// ELegendStyle
const unsigned long lsAuto = 0;
const unsigned long lsSeries = 1;
const unsigned long lsValues = 2;
const unsigned long lsLastValues = 3;
const unsigned long lsSeriesGroups = 4;

// ELegendSymbolPosition
const unsigned long spLeft = 0; 
const unsigned long spRight = 1;

// ELegendSymbolSize
const unsigned long lcsPercent = 0; 
const unsigned long lcsPixels = 1;

// ELegendTextStyle
const unsigned long ltsPlain = 0;
const unsigned long ltsLeftValue =1;
const unsigned long ltsRightValue =2;
const unsigned long ltsLeftPercent = 3;
const unsigned long ltsRightPercent = 4;
const unsigned long ltsXValue = 5;
const unsigned long ltsValue = 6;
const unsigned long ltsPercent = 7;
const unsigned long ltsXAndValue = 8;
const unsigned long ltsXAndPercent = 9;

// ELightStyle
const unsigned long lsLinear = 0;
const unsigned long lsSpotLight = 1;

// ELoadMode
const unsigned long lmClear = 0;
const unsigned long lmAppend = 1;

// EMarginUnits
const unsigned long muPercent = 0; 
const unsigned long muPixels = 1;

// EMarkStyle
const unsigned long smsValue = 0;
const unsigned long smsPercent = 1;
const unsigned long smsLabel = 2;
const unsigned long smsLabelPercent = 3;
const unsigned long smsLabelValue = 4;
const unsigned long smsLegend = 5;
const unsigned long smsPercentTotal = 6;
const unsigned long smsLabelPercentTotal = 7;
const unsigned long smsXValue = 8;
const unsigned long smsXY = 9;
const unsigned long smsSeriesTitle = 10;
const unsigned long smsPointIndex = 11;

// EMarkToolMouseAction
const unsigned long mtmMove = 0;
const unsigned long mtmClick = 1;

// EMirrorDirection
const unsigned long mdDown = 0;
const unsigned long mdUp = 1;
const unsigned long mdRight = 2;
const unsigned long mdLeft = 3;

// EMouseButton
const unsigned long mbLeft = 1;
const unsigned long mbRight = 2;
const unsigned long mbMiddle = 4;

// EMultiArea
const unsigned long maNone = 0;
const unsigned long maStacked = 1;
const unsigned long maStacked100 = 2;

// EMultiBar
const unsigned long mbNone = 0;
const unsigned long mbSide = 1;
const unsigned long mbStacked = 2;
const unsigned long mbStacked100 = 3;
const unsigned long mbSideAll = 4;
const unsigned long mbSelfStack = 5;

// EMUltiPie
const unsigned long mpAutomatic = 0;
const unsigned long mpDisabled = 1;

// ENearestToolStyle
const unsigned long hsNone = 0; 
const unsigned long hsCircle = 1;
const unsigned long hsRectangle = 2;
const unsigned long hsDiamond = 3;

// EOrgLineStyle
const unsigned lsSquared = 0;
const unsigned lsDiagonal = 1;

// EPaletteStyle
const unsigned long psPale = 0;
const unsigned long psStrong = 1;
const unsigned long psGrayScale = 2;
const unsigned long psInvGray = 3;
const unsigned long psRainbow = 4;
const unsigned long psCustom = 5;

// EPanelBevel
const unsigned long bvNone = 0;
const unsigned long bvLowered = 1;
const unsigned long bvRaised = 2;

// EPenEndStyle 
const unsigned long esRound = 0;
const unsigned long esSquare = 1;
const unsigned long esFlat = 2;

// EPenLineMode
const unsigned long lmLine = 0;
const unsigned long lmCylinder = 1;
const unsigned long lmRectangle = 2;

// EPenMode
const unsigned long pmBlack = 0;
const unsigned long pmWhite = 1;
const unsigned long pmNop = 2;
const unsigned long pmNot = 3;
const unsigned long pmCopy = 4;
const unsigned long pmNotCopy = 5;
const unsigned long pmMergePenNot = 6;
const unsigned long pmMaskPenNot = 7;
const unsigned long pmMergeNotPen = 8;
const unsigned long pmMaskNotPen = 9;
const unsigned long pmMerge = 10;
const unsigned long pmNotMerge = 11;
const unsigned long pmMask = 12;
const unsigned long pmNotMask = 13;
const unsigned long pmXor = 14;
const unsigned long pmNotXor = 15;

// EPieOtherStyle 
const unsigned long poNone = 0;
const unsigned long poBelowPercent = 1;
const unsigned long poBelowValue = 2;

// EPieToolStyle
const unsigned long ptFocus = 0;
const unsigned long ptExplode = 1;

// EPixelFormat
const unsigned long pfDevice = 0;
const unsigned long pf1bit = 1;
const unsigned long pf4bit = 2;
const unsigned long pf8bit = 3;
const unsigned long pf15bit = 4;
const unsigned long pf16bit = 5;
const unsigned long pf24bit = 6;
const unsigned long pf32bit = 7;

// EPointerStyle
const unsigned long psRectangle = 0;
const unsigned long psCircle = 1;
const unsigned long psTriangle = 2;
const unsigned long psDownTriangle = 3;
const unsigned long psCross = 4;
const unsigned long psDiagCross = 5;
const unsigned long psStar = 6;
const unsigned long psDiamond = 7;
const unsigned long psSmallDot = 8;
const unsigned long psNothing = 9;
const unsigned long psLeftTriangle = 10;
const unsigned long psRightTriangle = 11;
const unsigned long psHexagon = 12;

// EPositionUnits
const unsigned long puPercent = 0;
const unsigned long puPixels = 1;

// EPrinterOrientation
const unsigned long poPortrait = 0;
const unsigned long poLandscape = 1;

// ERotateToolStyle
const unsigned long rsAll = 0;
const unsigned long rsRotation = 1;
const unsigned long rsElevation = 2;

// ERSIStyle
const unsigned long rsiOpenClose = 0; 
const unsigned long rsiClose = 1;

// EScopeCursorStyle
const unsigned long scsRectangle = 0;
const unsigned long scsCircle = 1;
const unsigned long scsDiamond = 2;
const unsigned long scsEmpty = 3;
const unsigned long scsCamera = 4;

// EScrollBarDrawStyle
const unsigned long dsAlways = 0;
const unsigned long dsWhenNeeded = 1;

// EScrollDirection
const unsigned long sdRightLeft = 0;
const unsigned long sdLeftRight = 1;

// EScrollThumbStyle
const unsigned long stsCustom = 0;
const unsigned long stsThumb1 = 1;
const unsigned long stsThumb2 = 2;
const unsigned long stsThumb3 = 3;

// ESeriesAnimationLoop
const unsigned long salNo = 0;
const unsigned long salOneWay = 1;
const unsigned long salCircular = 2;

// ESeriesClass
const unsigned long scLine = 0;
const unsigned long scBar = 1;
const unsigned long scHorizBar = 2;
const unsigned long scArea = 3;
const unsigned long scPoint = 4;
const unsigned long scPie = 5;
const unsigned long scFastLine = 6;
const unsigned long scShape = 7;
const unsigned long scGantt = 8;
const unsigned long scBubble = 9;
const unsigned long scArrow = 10;
const unsigned long scCandle = 11;
const unsigned long scPolar = 12;
const unsigned long scSurface = 13;
const unsigned long scVolume = 14;
const unsigned long scErrorBar = 15;
const unsigned long scBezier = 16;
const unsigned long scContour = 17;
const unsigned long scError = 18;
const unsigned long scPoint3D = 19;
const unsigned long scRadar = 20;
const unsigned long scClock = 21; 
const unsigned long scWindRose= 22;  
const unsigned long scBar3D = 23;  
const unsigned long scImageBar = 24;  
const unsigned long scDonut = 25; 
const unsigned long scTriSurface = 26; 
const unsigned long scBox = 27; 
const unsigned long scHorizBox = 28; 
const unsigned long scHistogram = 29; 
const unsigned long scColorGrid = 30; 
const unsigned long scBarJoin = 31; 
const unsigned long scHighLow = 32; 
const unsigned long scWaterfall = 33; 
const unsigned long scSmith = 34; 
const unsigned long scPyramid = 35; 
const unsigned long scMap = 36;
const unsigned long scHorizLine = 37;
const unsigned long scFunnel = 38;
const unsigned long scCalendar = 39;
const unsigned long scHorizArea = 40;
const unsigned long scPointFigure = 41;
const unsigned long scGauge = 42;
const unsigned long scVector3D = 43;
const unsigned long scTower = 44;
const unsigned long scPolarBar = 45;
const unsigned long scBubble3D = 46;
const unsigned long scHorizHistogram = 47;
const unsigned long scVolumePipe = 48;
const unsigned long scIsoSurface = 49;
const unsigned long scDarvas = 50;
const unsigned long scHighLowLine = 51;
const unsigned long scPolarGrid = 52;
const unsigned long scDeltaPoint = 53;
const unsigned long scImagePoint = 54;
const unsigned long scOrganizational = 55;
const unsigned long scWorld = 56;
const unsigned long scTagCloud = 57;
const unsigned long scKagi = 58;
const unsigned long scRenko = 59;
const unsigned long scNumericGauge = 60;
const unsigned long scLinearGauge = 61;
const unsigned long scCircularGauge = 62;
const unsigned long scBigCandle = 63;
const unsigned long scLinePoint = 64;

// ESeriesGroupActive
const unsigned long gaYes = 0;
const unsigned long gaNo = 1;
const unsigned long gaSome = 2;

// EShapeStyle
const unsigned long chasRectangle = 0;
const unsigned long chasCircle = 1;
const unsigned long chasVertLine = 2;
const unsigned long chasHorizLine = 3;
const unsigned long chasTriangle = 4;
const unsigned long chasInvertTriangle = 5;
const unsigned long chasLine = 6;
const unsigned long chasDiamond = 7;
const unsigned long chasCube = 8;
const unsigned long chasCross = 9;
const unsigned long chasDiagCross = 10;
const unsigned long chasStar = 11;
const unsigned long chasPyramid = 12;
const unsigned long chasInvertPyramid = 13;

// EShapeXYStyle
const unsigned long xysPixels = 0;
const unsigned long xysAxis = 1;
const unsigned long xysAxisOrigin = 2;

// EShiftState
const unsigned long ssShift = 1;
const unsigned long ssAlt = 2;
const unsigned long ssCtrl = 4;
const unsigned long ssNone = 0;

// ESnapStyle
const unsigned long ssDefault = 0;
const unsigned long ssHorizontal = 1;
const unsigned long ssVertical = 2;

// EStreamType
const unsigned long stJpeg = 0;
const unsigned long stTeeTemplate = 1;

// ETagCloudOrder
const unsigned long toText = 0;
const unsigned long toValue = 1;
const unsigned long toNone = 2;

// ETeeCommanderControls
const unsigned long tcbNormal = 0; 
const unsigned long tcbSeparator = 1;
const unsigned long tcbRotate = 2; 
const unsigned long tcbMove = 3; 
const unsigned long tcbZoom = 4; 
const unsigned long tcbDepth = 5; 
const unsigned long tcbEdit = 6; 
const unsigned long tcbPrintPreview = 7;
const unsigned long tcbCopy = 8; 
const unsigned long tcbSave = 9; 
const unsigned long tcbLabel = 10;
const unsigned long tcb3D = 11;

// ETeeEditorPage
const unsigned long epMain = 0;
const unsigned long epGeneral = 1;
const unsigned long epAxis = 2;
const unsigned long epTitles = 3;
const unsigned long epLegend = 4;
const unsigned long epPanel = 5;
const unsigned long epPaging = 6;
const unsigned long epWalls = 7;
const unsigned long epAspect = 8;
const unsigned long epSeriesGeneral = 9;
const unsigned long epSeriesMarks = 10; 
const unsigned long epAllSeries = 11; 
const unsigned long epSeriesData = 12; 
const unsigned long epExport = 13; 
const unsigned long epTools = 14; 
const unsigned long epPrintPreview = 15;
const unsigned long epOpenGL = 16;

// ETeePreviewPanelOrientation
const unsigned long ppoDefault = 0;
const unsigned long ppoPortrait = 1;
const unsigned long ppoLandscape = 2;

// ETeeVertAlign
const unsigned long vaTop = 0; 
const unsigned long vaCenter = 1; 
const unsigned long vaBottom = 2;

// ETeeZoomDirection
const unsigned long tzdHorizontal = 0;
const unsigned long tzdVertical = 1;
const unsigned long tzdBoth = 2;

// ETextAlignment
const unsigned long aLeft = 0;
const unsigned long aRight = 1;
const unsigned long aCenter = 2;

// ETitleAlignment
const unsigned long taLeftJustify = 0;
const unsigned long taRightJustify = 1;
const unsigned long taCenter = 2;

// EToolClass
const unsigned long tcCursor = 0; 
const unsigned long tcDragMarks = 1; 
const unsigned long tcAxisArrow = 2; 
const unsigned long tcDrawLine = 3; 
const unsigned long tcNearest = 4; 
const unsigned long tcColorband = 5; 
const unsigned long tcColorLine = 6; 
const unsigned long tcRotate = 7;
const unsigned long tcMarksTip = 8;
const unsigned long tcChartImage = 9;
const unsigned long tcAnnotate = 10;
const unsigned long tcPageNumber = 11;
const unsigned long tcGridTranspose = 12;
const unsigned long tcExtraLegend = 13;
const unsigned long tcSeriesAnimation = 14;
const unsigned long tcGantt = 15;
const unsigned long tcGridBand = 16;
const unsigned long tcPie = 17;
const unsigned long tcDragPoint = 18;
const unsigned long tcLegendScrollBar = 19;
const unsigned long tcLight = 20;
const unsigned long tcSurfaceNearest = 21;
const unsigned long tcAxisScroll = 22;
const unsigned long tcSeriesBand = 23;
const unsigned long tcSelector = 24;
const unsigned long tcRectangle = 25;
const unsigned long tcDataTable = 26;
const unsigned long tcClipSeries = 27;
const unsigned long tcSeriesStats = 28;
const unsigned long tcVideo = 29;
const unsigned long tcVideoPlayer = 30;
const unsigned long tcTransposeSeries = 31;
const unsigned long tcSeriesRegion = 32;
const unsigned long tcAntiAlias = 33;
const unsigned long tcFader = 34;
const unsigned long tcFrame = 35;
const unsigned long tcMagnify = 36;
const unsigned long tcLegendPalette = 37;
const unsigned long tcBanner = 38;
const unsigned long tcText3D = 39;
const unsigned long tcSubChart = 40;
const unsigned long tcLink = 41;

// ETowerStyle
const unsigned long tsCube = 0;
const unsigned long tsRectangle = 1;
const unsigned long tsCover = 2;
const unsigned long tsCylinder = 3;
const unsigned long tsArrow = 4;
const unsigned long tsCone = 5;
const unsigned long tsPyramid = 6;
const unsigned long tsSlantCube = 7;

// ETreatNullsStyle
const unsigned long tnDontPaint = 0 ;
const unsigned long tnSkip = 1;
const unsigned long tnIgnore = 2;

// ETypeFitting
const unsigned long cfPolynomial = 0;
const unsigned long cfLogarithmic = 1;
const unsigned long cfExponential = 2;

// EValueListOrder
const unsigned long loNone = 0;
const unsigned long loAscending = 1;
const unsigned long loDescending = 2;

// EVerticalAxis
const unsigned long aLeftAxis = 0;
const unsigned long aRightAxis = 1;
const unsigned long aBothVertAxis = 2;
const unsigned long aCustomVertAxis = 3;

// EWorldMap
const unsigned long wmWorld = 0;
const unsigned long wmAfrica = 1;
const unsigned long wmAsia = 2;
const unsigned long wmAustralia = 3;
const unsigned long wmCentralAmerica = 4;
const unsigned long wmEurope = 5;
const unsigned long wmEurope15 = 6;
const unsigned long wmEurope27 = 7;
const unsigned long wmSpain = 8;
const unsigned long wmMiddleEast = 9;
const unsigned long wmNorthAmerica = 10;
const unsigned long wmSouthAmerica = 11;
const unsigned long wmUSA = 12;
const unsigned long wmUSAHawaiiAlaska = 13;


#endif
