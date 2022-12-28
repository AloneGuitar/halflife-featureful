#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include "r_efx.h"
#include "customentity.h"
#include "r_studioint.h"
#include "event_api.h"
#include "com_model.h"
#include "studio.h"
#include "pmtrace.h"
#include "cl_msg.h"

#include "r_studioint.h"

extern engine_studio_api_t IEngineStudio;

void GibHitCallback( TEMPENTITY* ent, pmtrace_t* pmtrace )
{
	static const char* redBloodDecals[] = {"{blood1", "{blood2", "{blood3", "{blood4", "{blood5", "{blood6"};
	static const char* yellowBloodDecals[] = {"{yblood1", "{yblood2", "{yblood3", "{yblood4", "{yblood5", "{yblood6"};

	const char* decalName = NULL;
	if (ent->entity.curstate.iuser1 == 1)
	{
		decalName = redBloodDecals[gEngfuncs.pfnRandomLong(0, 5)];
	}
	else if (ent->entity.curstate.iuser1 > 1)
	{
		decalName = yellowBloodDecals[gEngfuncs.pfnRandomLong(0, 5)];
	}

	if (ent->entity.curstate.onground)
	{
		ent->entity.baseline.origin = ent->entity.baseline.origin * 0.9;
		ent->entity.curstate.angles.x = 0;
		ent->entity.curstate.angles.z = 0;
		ent->entity.baseline.angles.x = 0;
		ent->entity.baseline.angles.z = 0;
	}
	else
	{
		ent->entity.curstate.origin = ent->entity.curstate.origin + Vector(0, 0, 8);

		if (ent->entity.curstate.iuser1 > 0 && ent->entity.curstate.iuser2 > 0 && decalName != NULL)
		{
			int decalIndex = gEngfuncs.pEfxAPI->Draw_DecalIndexFromName( (char*)decalName );
			int textureIndex = gEngfuncs.pEfxAPI->Draw_DecalIndex( decalIndex );
			int traceEntIndex = gEngfuncs.pEventAPI->EV_IndexFromTrace( pmtrace );
			gEngfuncs.pEfxAPI->R_DecalShoot(textureIndex, traceEntIndex, 0, pmtrace->endpos, 0 );
			ent->entity.curstate.iuser2--;
		}
	}
}

int __MsgFunc_RandomGibs( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	Vector absmin;
	Vector size;
	Vector direction;

	absmin[0] = READ_COORD();
	absmin[1] = READ_COORD();
	absmin[2] = READ_COORD();

	size[0] = READ_COORD();
	size[1] = READ_COORD();
	size[2] = READ_COORD();

	direction[0] = READ_COORD();
	direction[1] = READ_COORD();
	direction[2] = READ_COORD();

	float randomization = READ_BYTE() / 100.0f;
	int modelIndex = READ_SHORT();
	int gibCount = READ_BYTE();
	int lifeTime = READ_BYTE();
	int bloodType = READ_BYTE();
	int gibBodiesNum = READ_BYTE();
	int startGibIndex = READ_BYTE();

	float velocityMultiplier = READ_BYTE() / 10.0f;

	struct model_s* model = IEngineStudio.GetModelByIndex(modelIndex);

	if (gibBodiesNum == 0)
	{
		studiohdr_t* pstudiohdr = (studiohdr_t *)IEngineStudio.Mod_Extradata(model);
		if (pstudiohdr)
		{
			mstudiobodyparts_t *pbodypart = (mstudiobodyparts_t *)( (byte *)pstudiohdr + pstudiohdr->bodypartindex );

			gibBodiesNum = 1;
			for (int j=0; j<pstudiohdr->numbodyparts; ++j)
			{
				gibBodiesNum = gibBodiesNum * pbodypart[j].nummodels;
			}
		}
	}

	if (gibBodiesNum == 0)
		gibBodiesNum = startGibIndex + 1;
	startGibIndex = startGibIndex > gibBodiesNum - 1 ? gibBodiesNum - 1 : startGibIndex;

	const float clientTime = gEngfuncs.GetClientTime();

	for (int i=0; i<gibCount; ++i)
	{
		Vector gibPos;
		gibPos.x = absmin.x + size.x * gEngfuncs.pfnRandomFloat(0, 1);
		gibPos.y = absmin.y + size.y * gEngfuncs.pfnRandomFloat(0, 1);
		gibPos.z = absmin.z + size.z * gEngfuncs.pfnRandomFloat(0, 1) + 1;

		Vector gibVelocity = direction;
		gibVelocity.x += gEngfuncs.pfnRandomFloat(-randomization, randomization);
		gibVelocity.y += gEngfuncs.pfnRandomFloat(-randomization, randomization);
		gibVelocity.z += gEngfuncs.pfnRandomFloat(-randomization, randomization);

		gibVelocity = gibVelocity * gEngfuncs.pfnRandomFloat( 300, 400 ) * velocityMultiplier;

		if (gibVelocity.Length() > 1500)
		{
			gibVelocity = gibVelocity.Normalize() * 1500;
		}

		TEMPENTITY* pTemp = gEngfuncs.pEfxAPI->CL_TempEntAlloc(gibPos, model);
		if (!pTemp)
			break;

		pTemp->entity.curstate.body = gEngfuncs.pfnRandomLong(startGibIndex, gibBodiesNum - 1);
		pTemp->flags |= FTENT_COLLIDEWORLD | FTENT_FADEOUT | FTENT_GRAVITY | FTENT_ROTATE | FTENT_PERSIST;

		pTemp->entity.curstate.iuser1 = bloodType;
		pTemp->entity.curstate.iuser2 = 5;
		pTemp->entity.curstate.solid = SOLID_SLIDEBOX;
		pTemp->entity.curstate.movetype = MOVETYPE_BOUNCE;
		pTemp->entity.curstate.friction = 0.55;
		pTemp->entity.curstate.rendermode = kRenderNormal;
		pTemp->entity.curstate.renderamt = pTemp->entity.baseline.renderamt = 255;
		pTemp->hitcallback = &GibHitCallback;

		pTemp->entity.baseline.angles.x = gEngfuncs.pfnRandomFloat(-256, 255);
		pTemp->entity.baseline.angles.z = gEngfuncs.pfnRandomFloat(-256, 255);
		pTemp->entity.baseline.origin = gibVelocity;
		pTemp->die = clientTime + lifeTime;
	}

	return 1;
}

int __MsgFunc_MuzzleLight( const char *pszName, int iSize, void *pbuf )
{
	Vector vecSrc;

	BEGIN_READ( pbuf, iSize );
	vecSrc[0] = READ_COORD();
	vecSrc[1] = READ_COORD();
	vecSrc[2] = READ_COORD();

	if (cl_muzzlelight_monsters && cl_muzzlelight_monsters->value)
	{
		dlight_t* dl = gEngfuncs.pEfxAPI->CL_AllocDlight( 0 );
		dl->origin[0] = vecSrc.x;
		dl->origin[1] = vecSrc.y;
		dl->origin[2] = vecSrc.z;
		dl->radius = 144;
		dl->color.r = 255;
		dl->color.g = 208;
		dl->color.b = 128;
		dl->die = gEngfuncs.GetClientTime() + 0.01f;
	}

	return 1;
}

int __MsgFunc_CustomBeam( const char* pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	int beamType = READ_BYTE();

	vec3_t	start, end;
	int	modelIndex, startFrame;
	float	frameRate, life, width;
	int	startEnt, endEnt;
	float	noise, speed;
	float	r, g, b, a;
	BEAM* beam;
	int flags;

	switch (beamType) {
	case TE_BEAMPOINTS:
	case TE_BEAMENTPOINT:
	case TE_BEAMENTS:
	case TE_BEAMRING:
	{
		if( beamType == TE_BEAMENTS || beamType == TE_BEAMRING )
		{
			startEnt = READ_SHORT();
			endEnt = READ_SHORT();
		}
		else
		{
			if( beamType == TE_BEAMENTPOINT )
			{
				startEnt = READ_SHORT();
			}
			else
			{
				start[0] = READ_COORD();
				start[1] = READ_COORD();
				start[2] = READ_COORD();
			}
			end[0] = READ_COORD();
			end[1] = READ_COORD();
			end[2] = READ_COORD();
		}
		modelIndex = READ_SHORT();
		startFrame = READ_BYTE();
		frameRate = (float)READ_BYTE() * 0.1f;
		life = (float)READ_BYTE() * 0.1f;
		width = (float)READ_BYTE() * 0.1f;
		noise = (float)READ_BYTE() * 0.01f;
		r = (float)READ_BYTE() / 255.0f;
		g = (float)READ_BYTE() / 255.0f;
		b = (float)READ_BYTE() / 255.0f;
		a = (float)READ_BYTE() / 255.0f;
		speed = (float)READ_BYTE() * 0.1f;
		flags = READ_BYTE();
		if ( beamType == TE_BEAMRING )
			beam = gEngfuncs.pEfxAPI->R_BeamRing( startEnt, endEnt, modelIndex, life, width, noise, a, speed, startFrame, frameRate, r, g, b );
		if( beamType == TE_BEAMENTS )
			beam = gEngfuncs.pEfxAPI->R_BeamEnts( startEnt, endEnt, modelIndex, life, width, noise, a, speed, startFrame, frameRate, r, g, b );
		else if( beamType == TE_BEAMENTPOINT )
			beam = gEngfuncs.pEfxAPI->R_BeamEntPoint( startEnt, end, modelIndex, life, width, noise, a, speed, startFrame, frameRate, r, g, b );
		else
			beam = gEngfuncs.pEfxAPI->R_BeamPoints( start, end, modelIndex, life, width, noise, a, speed, startFrame, frameRate, r, g, b );
		if (beam)
		{
			if (flags & BEAM_FSINE)
				beam->flags |= FBEAM_SINENOISE;
			if (flags & BEAM_FSOLID)
				beam->flags |= FBEAM_SOLID;
			if (flags & BEAM_FSHADEIN)
				beam->flags |= FBEAM_SHADEIN;
			if (flags & BEAM_FSHADEOUT)
				beam->flags |= FBEAM_SHADEOUT;
		}
	}
		break;
	default:
		gEngfuncs.Con_DPrintf("%s: got the unknown beam type %d\n", pszName, beamType);
		break;
	}

	return 1;
}

void HookFXMessages()
{
	gEngfuncs.pfnHookUserMsg( "RandomGibs", __MsgFunc_RandomGibs );
	gEngfuncs.pfnHookUserMsg( "MuzzleLight", __MsgFunc_MuzzleLight );
	gEngfuncs.pfnHookUserMsg( "CustomBeam", __MsgFunc_CustomBeam );
}
