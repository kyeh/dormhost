class CreateMarkers < ActiveRecord::Migration
  def self.up
    create_table :markers do |t|
      t.column :room_id, :integer, :null=>false
      t.column :lat, :decimal, :precision=>15, :scale=>10
      t.column :lng, :decimal, :precision=>15, :scale=>10
      t.column :details, :string, :limit=>200
      t.timestamps
    end
  end

  def self.down
    drop_table :markers
  end
end
