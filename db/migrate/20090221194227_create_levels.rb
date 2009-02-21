class CreateLevels < ActiveRecord::Migration
  def self.up
    create_table :levels do |t|
      t.column  :rank,        :integer
      t.column  :cleanliness, :string,  :limit => 50

      t.timestamps
    end
  end

  def self.down
    drop_table :levels
  end
end
