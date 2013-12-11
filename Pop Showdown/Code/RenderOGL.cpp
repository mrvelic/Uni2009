#include "common.h"
#include "RenderOGL.h"

void RenderOGL::Draw() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		if(m_CameraSet) {
			glTranslatef(m_CurrentCamera->X() / drawRatio, m_CurrentCamera->Y() / drawRatio, m_CurrentCamera->Z() / drawRatio);
			DrawBackground();
		}

		if(m_EntitiesSet) {
			DrawEntities(m_Entities, false, NULL);
		}
}

void RenderOGL::DrawEntities(vector<Entity*>* ents, bool relative, Entity* relEnt) {
	Entity* current_ent;

	for(unsigned int i = 0; i < ents->size(); i++) {
		current_ent = ents->at(i);
		float y_pos = 0.0f;
		if(relative) {
			y_pos = current_ent->Y() + relEnt->Y();
		} else {
			y_pos = current_ent->Y();
		}

		if(current_ent->DrawFromBottom()) {
			if(current_ent->HasSprite()) y_pos = y_pos + (current_ent->GetSprite()->Height() / 2);
		}

		glPushMatrix();
			if(relative) {
				if(relEnt->Direction() == 1) {
					glTranslatef((-current_ent->X() + relEnt->X()) / drawRatio, y_pos / drawRatio, (current_ent->Z() + relEnt->Z()) / drawRatio);
					if(!current_ent->IgnoreDirection()) current_ent->Yaw(180.0f);
				} else {
					glTranslatef((current_ent->X() + relEnt->X()) / drawRatio, y_pos / drawRatio, (current_ent->Z() + relEnt->Z()) / drawRatio);
					if(!current_ent->IgnoreDirection()) current_ent->Yaw(0.0f);
				}
			} else {
				glTranslatef(current_ent->X() / drawRatio, y_pos / drawRatio, current_ent->Z() / drawRatio);
			}
			glRotatef(current_ent->Roll(), 0.0f, 0.0f, 1.0f);
			glRotatef(current_ent->Yaw(), 0.0f, 1.0f, 0.0f);
			glRotatef(current_ent->Pitch(), 1.0f, 0.0f, 0.0f);
			if(current_ent->HasSprite()) DrawSprite(current_ent->GetSprite(), current_ent->Scale());
			if(current_ent->HasModel()) DrawModel(current_ent->GetModel());
		glPopMatrix();

		DrawEntities(current_ent->GetEntities(), true, current_ent);
	}
}

void RenderOGL::DrawSprite(Sprite* sprite, float scale) {
	// set colour to white
	glColor3f( 1.0f, 1.0f, 1.0f );
	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// bind the texture
	glBindTexture(GL_TEXTURE_2D, sprite->Texture()->ID());

	float hWidth = sprite->Width() / 2;
	float hHeight = sprite->Height() / 2;

	// draw the quad
	glBegin( GL_QUADS );
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-(hWidth) / drawRatio, -(hHeight) / drawRatio,  0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( hWidth / drawRatio, -(hHeight) / drawRatio,  0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( hWidth / drawRatio, hHeight / drawRatio,  0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-(hWidth) / drawRatio,  hHeight / drawRatio,  0.0f);
		//glScalef(scale, scale, 0.0f);
	glEnd();

	glDisable(GL_BLEND);
}

void RenderOGL::DrawBackground() {
	// draw the quad
	LevelBackground* bg = m_Background;
	Camera* c = m_CurrentCamera;
	glPushMatrix();
	glTranslatef(-m_CurrentCamera->X() / drawRatio, 0.0f, 0.0f);

	glBegin( GL_QUADS );
		glColor3f(bg->topColour->GLr(), bg->topColour->GLg(), bg->topColour->GLb());
		glVertex3f(c->Z() / drawRatio, abs(c->Z() / drawRatio), 0.0f);
		glVertex3f(abs(c->Z() / drawRatio), abs(c->Z() / drawRatio), 0.0f);
		glColor3f(bg->bottomColour->GLr(), bg->bottomColour->GLg(), bg->bottomColour->GLb());
		glVertex3f(abs(c->Z() / drawRatio), c->Z() / drawRatio, 0.0f);
		glVertex3f(c->Z() / drawRatio, c->Z() / drawRatio, 0.0f);
	glEnd();
	glPopMatrix();
}

void RenderOGL::DrawModel(Model* model) {


}

void RenderOGL::DrawText(float x, float y, Colour colour, string text) {

	glPushMatrix();
		glColor3f(colour.GLr(), colour.GLg(), colour.GLb());
		//glTranslatef(x / drawRatio, y / drawRatio, 0.0f);
		glRasterPos2f(x / drawRatio, y / drawRatio);
		glPushAttrib(GL_LIST_BIT);
		glListBase(m_FontBase - 32);
		glCallLists(strlen(text.c_str()), GL_UNSIGNED_BYTE, text.c_str());
		glPopAttrib();
	glPopMatrix();
}

void RenderOGL::BuildFont() {
	HFONT font;
	m_FontBase = glGenLists(96);
	// create font
	font = CreateFont(-18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH, TEXT("Calibri"));
	SelectObject(m_Window->GetHDC(), font);
	// create gl list from font
	//wglUseFontOutlines(m_Window->GetHDC(), 0, 255, m_FontBase, 0.0f, 0.0f, WGL_FONT_POLYGONS, m_FontMetrics);
	wglUseFontBitmaps(m_Window->GetHDC(), 32, 96, m_FontBase);
}


void RenderOGL::Enable2D() {
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );
}

void RenderOGL::Disable2D() {
	glEnable( GL_DEPTH_TEST );
    glEnable( GL_LIGHTING );
}